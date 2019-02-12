#include "../../HeaderFiles/FileInfo.h"
#include "../../HeaderFiles/DBOperations.h"
#include "../../HeaderFiles/ProcessorQueue.h"

int FileInfo::noOfFiles = 0;
//std::map<std::string, std::streampos> FileInfo::FilePosMap;
std::vector<std::string> FileInfo::fileNamesMonitored;
std::vector<std::string> FileInfo::monitorDir;
std::map<std::string, std::vector<std::string>> FileInfo::dirIncludeTypeMap;
std::map<std::string, std::vector<std::string>> FileInfo::dirExcludeTypeMap;
std::map<std::string, std::string> FileInfo::dirPatternMap;

void FileInfo::AddFileNamesMonitored(std::string fileName)
{
  fileNamesMonitored.push_back(fileName);
  noOfFiles++;
}

void FileInfo::PrintFileNamesMonitored()
{
  for (std::string &names : fileNamesMonitored)
  {
     std::cout << names << " " << std::endl;
    ProcessorQueue::AddFileNamesMonitored(names);
  }
}

bool FileInfo::FindDirInMonitorDir(std::string dirName)
{
  if (find(monitorDir.begin(), monitorDir.end(), dirName) == monitorDir.end())
    return true;
  return false;
}

void FileInfo::PushMonitorDirectory(std::string dirName)
{
  monitorDir.push_back(dirName);
}

void FileInfo::PrintMonitorDirectory()
{
  std::cout << "The directories to monitor are ";
  for (std::string & s : monitorDir)
  {
    std::cout << s << " ";
  }
  std::cout<<std::endl;
}

//void FileInfo::InitializePosMap()
//{
//  for (std::string &s : fileNamesMonitored)
//  {
//    FilePosMap[s] = 0;
//  }
//}


int FileInfo::CountMonitorDirecs()
{
  return monitorDir.size();
}

std::vector<std::string> FileInfo::GetMonitorDirecsVector()
{
  return monitorDir;
}

void FileInfo::UpdateDB()
{
  for (std::string &s : fileNamesMonitored)
  {
    DBOperations::InsertQuery("insert or ignore into fileInfo values(?1,?2,?3)",s, 0, 0);
  }
}

void FileInfo::LoadDirectoryMap(std::string dirName, std::vector<std::string> includeTypesVector, std::vector<std::string> excludeTypesVector ,std::string pattern)
{
  dirIncludeTypeMap[dirName] = includeTypesVector;
  dirExcludeTypeMap[dirName] = excludeTypesVector;
  dirPatternMap[dirName] = pattern;
}

