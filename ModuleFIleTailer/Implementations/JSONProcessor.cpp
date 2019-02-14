#include "HeaderFiles/Includes.h"
#include "HeaderFiles/FileInfo.h"

std::string FindDir(std::string s)
{
  reverse(s.begin(), s.end());
  int index = s.find("\\", 0);
  std::string txt = s.substr(0, index);
  s.replace(0, index + 1, "");
  reverse(s.begin(), s.end());
  reverse(txt.begin(), txt.end());
  return s;
}
void CreateProcessPattern(std::string patternReference, std::map<std::string, std::string> &mapRef)
{
  rapidjson::Document d1;
  d1.Parse(patternReference.c_str());
  if (d1.IsArray())
  {
    for (const rapidjson::Value& outerArray : d1.GetArray())
    {
      if (outerArray.HasMember("LIST"))
      {
        for (const rapidjson::Value& innerListArray : outerArray["LIST"].GetArray())
        {
          mapRef[innerListArray["DATA"].GetString()] = innerListArray["regex"].GetString();
       //   std::cout << innerListArray["DATA"].GetString() << " " << innerListArray["regex"].GetString() << std::endl;
        }
      }
      else
      {
        std::cout << "Invalid Input for Pattern Reference...." << std::endl;
      }
    }
  }
  else
  {
    std::cout << "Invalid Input for Pattern Reference...." <<std::endl;
  }
}
void ProcessJSON(std::string inputJSON)
{
  std::map<std::string, std::string> patternMap;
  std::string patternReference = "[{ \"NAME\": \"Day Pattern\", \"HASCHILD\": true, \"LIST\": [{ \"NAME\": \"1,2,3,4...\", \"DATA\": \"${d}\", \"regex\": \"([1-9]|[12]\\\\d|3[01])\" }, { \"NAME\": \"01,02,03...\", \"DATA\": \"${dd}\", \"regex\": \"(0[1-9]|[12]\\\\d|3[01])\" }] }, { \"NAME\": \"Month Pattern\", \"HASCHILD\": true, \"LIST\": [{ \"NAME\": \"1,2,3,4...\", \"DATA\": \"${M}\", \"regex\": \"([1-9]|1[12])\" }, { \"NAME\": \"01,02,03...\", \"DATA\": \"${MM}\", \"regex\": \"(0[1-9]|1[12])\" }, { \"NAME\": \"JAN,FEB,MAR...\", \"DATA\": \"${MMM}\", \"regex\": \"jan|feb|mar|apr|may|jun|jul|aug|sep|oct|nov|dec\" }, { \"NAME\": \"JANUARY,FEBRUARY...\", \"DATA\": \"${MMMM}\", \"regex\": \"january|february|march|april|may|june|july|august|september|october|november|december\" }] }, { \"NAME\": \"Year Pattern\", \"HASCHILD\": true, \"LIST\": [{ \"NAME\": \"01,02,03...\", \"DATA\": \"${yy}\", \"regex\": \"\\\\d{2}\" }, { \"NAME\": \"2001,2002...\", \"DATA\": \"${yyyy}\", \"regex\": \"\\\\d{4}\" }] }, { \"NAME\": \"Weekday Pattern\", \"HASCHILD\": true, \"LIST\": [{ \"NAME\": \"Sun,Mon,Tue...\", \"DATA\": \"${EEE}\", \"regex\": \"sun|mon|tue|wed|thu|fri|sat\" }, { \"NAME\": \"Sunday,Mondays...\", \"DATA\": \"${EEEE}\", \"regex\": \"sunday|monday|tuesday|wednesday|thursday|friday\" }] }, { \"NAME\": \"Week in Year Pattern\", \"HASCHILD\": true, \"LIST\": [{ \"NAME\": \"1,2,3...\", \"DATA\": \"${w}\", \"regex\": \"([1-9]|[1234]\\\\d|5[012])\" }, { \"NAME\": \"01,02...\", \"DATA\": \"${ww}\", \"regex\": \"(0[1-9]|[1234]\\\\d|5[012])\" }] }, { \"NAME\": \"Week In Month Pattern\", \"HASCHILD\": true, \"LIST\": [{ \"NAME\": \"1,2,3...\", \"DATA\": \"${W}\", \"regex\": \"([1234])\" }, { \"NAME\": \"01,02...\", \"DATA\": \"${WW}\", \"regex\": \"(0[1234])\" }] }, { \"NAME\": \"Hour Pattern\", \"HASCHILD\": true, \"LIST\": [{ \"NAME\": \"1,2...24\", \"DATA\": \"${H}\", \"regex\": \"([1-9]|[1]\\\\d|2[01234])\" }, { \"NAME\": \"01,02...24\", \"DATA\": \"${HH}\", \"regex\": \"(0[1-9]|[1]\\\\d|2[01234])\" }] }, { \"NAME\": \"Minute Pattern\", \"HASCHILD\": true, \"LIST\": [{ \"NAME\": \"1,2,3,4...\", \"DATA\": \"${m}\", \"regex\": \"([1-9]|[12345]\\\\d)\" }, { \"NAME\": \"01,02...\", \"DATA\": \"${mm}\", \"regex\": \"(0[1-9]|[12345]\\\\d)\" }] }, { \"NAME\": \"Second Pattern\", \"HASCHILD\": true, \"LIST\": [{ \"NAME\": \"1,2,3,4...\", \"DATA\": \"${s}\", \"regex\": \"([1-9]|[12345]\\\\d)\" }, { \"NAME\": \"01,02...\", \"DATA\": \"${ss}\", \"regex\": \"(0[1-9]|[12345]\\\\d)\" }] }, { \"NAME\": \"Number of Increments Pattern\", \"HASCHILD\": true, \"LIST\": [{ \"NAME\": \"1,2,3,4...\", \"DATA\": \"${{NUMBER_1}}\", \"regex\": \"(\\\\d+)\" }, { \"NAME\": \"01,02,03...\", \"DATA\": \"${{NUMBER_2}}\", \"regex\": \"(0[0-9]|[1-9]\\\\d+)\" }, { \"NAME\": \"001,002,003...\", \"DATA\": \"${{NUMBER_3}}\", \"regex\": \"(00[0-9]|0[1-9][0-9]|[1-9][0-9]\\\\d+)\" }] }]";
  CreateProcessPattern(patternReference, patternMap);
  std::vector<std::string> excludeDir;
  std::vector<std::string> excludeFiles;
  std::queue<std::string> includeDir;
  std::vector<std::string> includeFiles;
  rapidjson::Document d;
  d.Parse(inputJSON.c_str());
  if (d.IsArray())
  {
    for (const rapidjson::Value& outerArray : d.GetArray())
    {
      if (outerArray.HasMember("locations"))
      {
        for (const rapidjson::Value& innerLocationsArray : outerArray["locations"].GetArray())
        {
          bool subDir = true;
          std::vector<std::string> excludeTypes;
          std::vector<std::string> includeTypes;
          if (innerLocationsArray.HasMember("excludetypes"))
          {
            for (const rapidjson::Value & innerExcludeTypesArray : innerLocationsArray["excludetypes"].GetArray())
            {
              excludeTypes.push_back(innerExcludeTypesArray.GetString());
            }
          }
          if (innerLocationsArray.HasMember("includetypes"))
          {
            for (const rapidjson::Value& innerIncludeTyepesArray : innerLocationsArray["includetypes"].GetArray())
            {
              includeTypes.push_back(innerIncludeTyepesArray.GetString());
            }
          }
          if (innerLocationsArray.HasMember("exclude"))
          {
            for (const rapidjson::Value & innerExcludeArray : innerLocationsArray["exclude"].GetArray())
            {
              std::string curName = innerExcludeArray.GetString();
              if (GetFileAttributes(curName.c_str()) == FILE_ATTRIBUTE_DIRECTORY)
              {
                excludeDir.push_back(curName);
              }
              else
              {
                if (curName == "*")
                {
                  subDir = false;
                }
                else
                {
                  excludeFiles.push_back(curName);
                }
              }
            }
          }
          if (innerLocationsArray.HasMember("include"))
          {
            std::string dirName = innerLocationsArray["include"].GetString();
            if (GetFileAttributes(dirName.c_str()) == FILE_ATTRIBUTE_DIRECTORY)
            {
              includeDir.push(dirName);
              std::string patternForDir = "";
              if (innerLocationsArray.HasMember("pattern"))
              {
                patternForDir = innerLocationsArray["pattern"].GetString();
              }
              while(!includeDir.empty())
              {
                std::string curDir = includeDir.front();
                FindFiles(curDir, excludeTypes, includeTypes, includeFiles, excludeFiles, includeDir, excludeDir,patternForDir,patternMap);
                includeDir.pop();
                if (subDir == false)
                {
                  break;
                }
              }
            }
            else
            {
              includeFiles.push_back(dirName);
              dirName = FindDir(dirName);
              FileInfo::PushMonitorDirectory(dirName);
            }
          }
        }
      }
      else
      {
        std::cout << "Has no member Locations";
      }
    }
    for (std::string &s: includeFiles)
    {
      FileInfo::AddFileNamesMonitored(s);
    }
  }
  else
  {
    std::cout << inputJSON;
  }
}