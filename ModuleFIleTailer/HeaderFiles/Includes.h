#pragma once

#pragma once
#pragma once



//headers
#include<iostream>
#include<string>
#include<thread>
#include<mutex>
#include<Windows.h>
#include<queue>
#include<vector>
#include<set>
#include<fstream>
#include<atlstr.h>
#include<map>
#include<regex>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

void FindFiles(std::string dir, std::vector<std::string> &excludetypes, std::vector<std::string> &includetypes, std::vector<std::string> &includefiles, std::vector<std::string> &excludefiles, std::queue<std::string> &includedir, std::vector<std::string> &excludedir,std::string patternForDir, std::map<std::string,std::string> patternMap);
void ProcessJSON(std::string s);
void IntializeThreadPoolInFileProcessor();


#define noOfThreads 4