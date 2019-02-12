#include "../../HeaderFiles/ProcessorQueue.h"
#include "../../HeaderFiles/DBOperations.h"

std::queue<std::string> ProcessorQueue::fileNamesToBeProcessed;
std::map<std::string, int> ProcessorQueue::FileProcessMap;
int ProcessorQueue::fileProcessMapSize;

std::condition_variable ProcessorQueue::cv;
std::mutex ProcessorQueue::mut;

void ProcessorQueue::push(std::string fileName)
{
  std::unique_lock<std::mutex> lck(ProcessorQueue::mut);
  if (FileProcessMap.find(fileName) != FileProcessMap.end())
  {
    if (FileProcessMap[fileName] == 0)
    {
      fileNamesToBeProcessed.push(fileName);
      FileProcessMap[fileName] = 1;
      fileProcessMapSize++;
      if (fileProcessMapSize <= 4)
      {
        ProcessorQueue::cv.notify_one();
      }
    }
  }
}

void ProcessorQueue::AddFileNamesMonitored(std::string fileName)
{
  std::unique_lock<std::mutex> lck(ProcessorQueue::mut);
  FileProcessMap[fileName] = 0;
  fileNamesToBeProcessed.push(fileName);
  FileProcessMap[fileName] = 1;
  fileProcessMapSize++;
  if (fileProcessMapSize <= 4)
  {
    ProcessorQueue::cv.notify_one();
  }
}


std::string ProcessorQueue::pop()
{
  std::unique_lock<std::mutex> lck(ProcessorQueue::mut);
  if (fileNamesToBeProcessed.size() > 0)
  {
    std::string name;
    name = fileNamesToBeProcessed.front();
    fileNamesToBeProcessed.pop();
    return name;
  }
  return NULL;
}

void ProcessorQueue::UnsetFromFileProcessMap(std::string fileName)
{
  std::unique_lock<std::mutex> lck(ProcessorQueue::mut);
  FileProcessMap[fileName] = 0;
  fileProcessMapSize--;
}


void ProcessorQueue::WorkerThreads(int threadNo)
{
  while (true) {
    std::unique_lock<std::mutex> lck(mut);
    while (ProcessorQueue::fileProcessMapSize == 0)
    {
      std::cout << threadNo << " waiting" << std::endl;
      cv.wait(lck);
      std::cout << threadNo << " released" << std::endl;
    }
    std::string curFile = "";
    if (ProcessorQueue::fileNamesToBeProcessed.size() > 0)
    {
      curFile = ProcessorQueue::fileNamesToBeProcessed.front();
      std::cout << curFile << std::endl;
      ProcessorQueue::fileNamesToBeProcessed.pop();
      lck.unlock();
      std::cout << "TO read " << curFile << threadNo << std::endl;
      ProcessorQueue::ReadFile(curFile);
      std::cout << " erased from existence " << threadNo << std::endl;
      std::unique_lock<std::mutex> lck(mut);
      if (ProcessorQueue::FileProcessMap[curFile] = 1) {
        ProcessorQueue::FileProcessMap[curFile] = 0;
        ProcessorQueue::fileProcessMapSize--;
      }
    }
  }
}

void ProcessorQueue::ReadFile(std::string fileName)
{
  long long streamposInt64;
  int skipValue;
  DBOperations::RetrieveSPosAndSkip("select streampos,skip from fileInfo where fileName = ?1", fileName, streamposInt64, skipValue);
  std::streampos streampos = streamposInt64;
  char *zErrMsg = 0;
  std::ifstream inputFile;
  inputFile.open(fileName);
  inputFile.seekg(streampos);
  std::ofstream outFile;
  std::string curLine;
  outFile.open("D:\\dir\\" + ProcessorQueue::FindFileName(fileName) + ".txt", std::ios_base::app);
  if (skipValue != 0)
  {
    std::getline(inputFile, curLine);
  }
  else
  {
    outFile << fileName << std::endl;
  }
  int recordCount = 0;
  while (inputFile.peek() != -1)
  {
    std::streampos curPos = inputFile.tellg();
    std::getline(inputFile, curLine);
    outFile << curLine << std::endl;
    recordCount++;
    if (inputFile.peek() == -1 || recordCount >= 1000)
    {
      recordCount = 0;
      DBOperations::UpdateQueury("update fileInfo set skip = ?1 , streampos = ?2 where fileName = ?3", curPos, 1, fileName);
    }
  }

}

std::string ProcessorQueue::FindFileName(std::string fileName)
{
  std::string temp;
  reverse(fileName.begin(), fileName.end());
  temp = fileName.substr(0, fileName.find("\\"));
  reverse(temp.begin(), temp.end());
  return temp;
}
