#pragma once
#include "Includes.h"
class FileInfo
{
  static int noOfFiles;
//  static std::map<std::string, std::streampos> FilePosMap;
  static std::vector<std::string> fileNamesMonitored;
  static std::vector<std::string> monitorDir;
  public:
    static std::map<std::string, std::vector<std::string>> dirIncludeTypeMap;
    static std::map<std::string, std::vector<std::string>> dirExcludeTypeMap;
    static std::map<std::string, std::string> dirPatternMap;
    static void AddFileNamesMonitored(std::string fileName);
    static void PrintFileNamesMonitored();
    static bool FindDirInMonitorDir(std::string dirName);
    static void PushMonitorDirectory(std::string dirName);
    static void PrintMonitorDirectory();
//    static void InitializePosMap();
    static int CountMonitorDirecs();
    static std::vector<std::string> GetMonitorDirecsVector();
    static void UpdateDB();
    static void LoadDirectoryMap(std::string dirName, std::vector<std::string> includeTypesVector, std::vector<std::string> excludeTypesVector, std::string pattern);
};