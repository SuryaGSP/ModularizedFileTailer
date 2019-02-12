#pragma once
#include "Includes.h"
#include "sqlite3.h"
class DBOperations
{
  static sqlite3 *DBObject;
public:
  static void OpenConnection();
  static void CloseConnection();
  static void UpdateQueury(std::string sql, long long int streampos, int SkipValue, std::string fileName);
  static void UpdateQuery(std::string sql, std::string fileName);
  static void InsertQuery(std::string sql, std::string fileName, int skipValue, long long streampos);
  static void RetrieveSPosAndSkip(std::string sql, std::string fileName, long long &streampos, int &skipValue);
};