#pragma once
#include "Includes.h"
class ProcessorQueue
{
  static std::queue<std::string> fileNamesToBeProcessed;
  static std::map<std::string, int> FileProcessMap;
public:
  static int fileProcessMapSize;
  static std::condition_variable cv;
  static std::mutex mut;

  static void push(std::string fileName);
  static void AddFileNamesMonitored(std::string fileName);
  static std::string pop();
  static void UnsetFromFileProcessMap(std::string fileName);
  //static int getFileNamesProcessedSize();
  static void WorkerThreads(int threadNo);
  static void ReadFile(std::string fileName);
  static std::string FindFileName(std::string fileName);
};