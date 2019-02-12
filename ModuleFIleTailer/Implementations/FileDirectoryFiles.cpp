#include "../HeaderFiles/Includes.h"
#include "../HeaderFiles/FileInfo.h"
std::string FindType(std::string fileName)
{
  reverse(fileName.begin(), fileName.end());
  int index = fileName.find(".", 0);
  std::string extension = fileName.substr(0, index + 1);
  reverse(extension.begin(), extension.end());
  return extension;
}

std::string ReplaceWithRegex(std::string input, std::map<std::string, std::string> &mapRef)
{
  std::string st = "(\\$\\{[^\\}]+\\})";
  std::regex regInstance(st);
  std::smatch match;
  int i = 0;
  std::string replace = "";
  while (std::regex_search(input, match, regInstance))
  {
    std::cout << match.str(i).length();
    std::cout << " " << match.str(i) << std::endl;
    input.replace(input.find(match.str(i), 0), match.str(i).length(), mapRef[match.str(i)]);
  }
  return ("(" + input + ")");
}

std::string RemoveExt(std::string filename)
{
  reverse(filename.begin(), filename.end());
  int index = filename.find(".", 0);
  std::string s = filename.substr(index + 1);
  reverse(s.begin(), s.end());
  return s;
}

bool CheckAndPush(std::string fileName, std::regex regInstance)
{
  std::string fileNameWithoutExt = RemoveExt(fileName);
  std::smatch m;
  return std::regex_match(fileNameWithoutExt, m, regInstance);
}


void FindFiles(std::string dirName, std::vector<std::string> &excludeTypes, std::vector<std::string> &includeTypes, std::vector<std::string> &includeFiles, std::vector<std::string> &excludeFiles, std::queue<std::string> &includeDir, std::vector<std::string> &excludeDir, std::string patternForDirectory, std::map<std::string, std::string> patternMap)
{
  std::string* fileNames = new std::string[100];
  WIN32_FIND_DATA data;
  std::string tempDir = dirName;
  tempDir = dirName + "*";
  HANDLE hFind = FindFirstFile(tempDir.c_str(), &data);
  if (hFind != INVALID_HANDLE_VALUE) {
    do {
      std::string name = data.cFileName;
      if (name == "." || name == "..");
      else if (data.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
      {
        if (find(excludeFiles.begin(), excludeFiles.end(), dirName + "*") == excludeFiles.end())
        {
          if (find(excludeDir.begin(), excludeDir.end(), dirName + name + "\\") == excludeDir.end())
          {
            includeDir.push(dirName + name + "\\");
          }
        }
      }
      else
      {
        if (find(excludeFiles.begin(), excludeFiles.end(), data.cFileName) == excludeFiles.end())
        {
          if (includeTypes.size() == 0 && excludeTypes.size() == 0)
          {
            if (patternForDirectory == "")
            {
              if (FileInfo::FindDirInMonitorDir(dirName))
              {
                FileInfo::PushMonitorDirectory(dirName);
                FileInfo::LoadDirectoryMap(dirName, includeTypes, excludeTypes, ReplaceWithRegex(patternForDirectory, patternMap));
              }
              includeFiles.push_back(dirName + data.cFileName);
            }
            else
            {
              std::regex regInstance(ReplaceWithRegex(patternForDirectory, patternMap));
              if (CheckAndPush(data.cFileName, regInstance))
              {
                if (FileInfo::FindDirInMonitorDir(dirName))
                {
                  FileInfo::LoadDirectoryMap(dirName, includeTypes, excludeTypes, ReplaceWithRegex(patternForDirectory, patternMap));
                  FileInfo::PushMonitorDirectory(dirName);
                }
                includeFiles.push_back(dirName + data.cFileName);
              }
            }
          }
          else
          {
            std::string fileType = FindType(data.cFileName);
            if ((includeTypes.size() > 0) && (find(includeTypes.begin(), includeTypes.end(), fileType) != includeTypes.end()))
            {
              if (patternForDirectory == "")
              {
                if (FileInfo::FindDirInMonitorDir(dirName))
                {
                  FileInfo::LoadDirectoryMap(dirName, includeTypes, excludeTypes, ReplaceWithRegex(patternForDirectory, patternMap));
                  FileInfo::PushMonitorDirectory(dirName);
                }
                includeFiles.push_back(dirName + data.cFileName);
              }
              else
              {
                std::regex regInstance(ReplaceWithRegex(patternForDirectory, patternMap));
                if (CheckAndPush(data.cFileName, regInstance))
                {
                  if (FileInfo::FindDirInMonitorDir(dirName))
                  {
                    FileInfo::LoadDirectoryMap(dirName, includeTypes, excludeTypes, ReplaceWithRegex(patternForDirectory, patternMap));
                    FileInfo::PushMonitorDirectory(dirName);
                  }
                  includeFiles.push_back(dirName + data.cFileName);
                }
              }
            }
            else if ((excludeTypes.size() > 0) && (find(excludeTypes.begin(), excludeTypes.end(), fileType) == excludeTypes.end()))
            {
              if (patternForDirectory == "")
              {
                if (FileInfo::FindDirInMonitorDir(dirName))
                {
                  FileInfo::LoadDirectoryMap(dirName, includeTypes, excludeTypes, ReplaceWithRegex(patternForDirectory, patternMap));
                  FileInfo::PushMonitorDirectory(dirName);
                }
                includeFiles.push_back(dirName + data.cFileName);
              }
              else
              {
                std::regex regInstance(ReplaceWithRegex(patternForDirectory, patternMap));
                if (CheckAndPush(data.cFileName, regInstance))
                {
                  if (FileInfo::FindDirInMonitorDir(dirName))
                  {
                    FileInfo::LoadDirectoryMap(dirName, includeTypes, excludeTypes, ReplaceWithRegex(patternForDirectory, patternMap));
                    FileInfo::PushMonitorDirectory(dirName);
                  }
                  includeFiles.push_back(dirName + data.cFileName);
                }
              }
            }
          }
        }
      }
    } while (FindNextFile(hFind, &data));
    FindClose(hFind);
  }
  return;
}