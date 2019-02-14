#include "HeaderFiles/DBOperations.h"


sqlite3 *DBOperations::DBObject = NULL;
void DBOperations::OpenConnection()
{
  if (sqlite3_open("stateTable.db", &DBObject) == SQLITE_OK)
  {
    std::cout << "connection established";
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(DBObject, "create table if not exists fileInfo(fileName text primary key , streampos INTEGER,skip INTEGER)", -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    std::cout << "Table created";
  }
  else
  {
    std::cout << "Database Connection failed";
  }
}

void DBOperations::CloseConnection()
{
  if (sqlite3_close(DBObject) == SQLITE_OK)
  {
      std::cout << "Connection closed";
  }
  else
  {
      std::cout << "Connection termination failed";
  }
}

void DBOperations::UpdateQueury(std::string sql, long long int streampos, int SkipValue, std::string fileName)
{
  sqlite3_stmt *stmt;
  sqlite3_prepare_v2(DBObject, sql.c_str(), -1, &stmt, NULL);
  sqlite3_bind_int64(stmt, 2, streampos);
  sqlite3_bind_int(stmt, 1, SkipValue);
  sqlite3_bind_text(stmt, 3, fileName.c_str(), -1, SQLITE_STATIC);
  sqlite3_step(stmt);
  sqlite3_finalize(stmt);
}

void DBOperations::UpdateQuery(std::string sql, std::string fileName)
{
  sqlite3_stmt *stmt;
  sqlite3_prepare_v2(DBObject, sql.c_str(), -1, &stmt, NULL);
  sqlite3_bind_text(stmt, 1, fileName.c_str(), -1, SQLITE_STATIC);
  sqlite3_step(stmt);
  sqlite3_finalize(stmt);
}


void DBOperations::RetrieveSPosAndSkip(std::string sql, std::string fileName, long long & streampos, int & skipValue)
{
  sqlite3_stmt *stmt;
  sqlite3_prepare_v2(DBObject, sql.c_str(), -1, &stmt, NULL);
  sqlite3_bind_text(stmt, 1, fileName.c_str(), -1, SQLITE_STATIC);
  if (sqlite3_step(stmt) != SQLITE_DONE)
  {
    streampos = sqlite3_column_int64(stmt, 0);
    skipValue = sqlite3_column_int(stmt, 1);
  }
  sqlite3_finalize(stmt);
  return;
}

void DBOperations::InsertQuery(std::string sql, std::string fileName, int skipValue, long long streampos)
{
  sqlite3_stmt *stmt;
  sqlite3_prepare_v2(DBObject, sql.c_str(), -1, &stmt, NULL);
  sqlite3_bind_int64(stmt, 2, streampos);
  sqlite3_bind_int(stmt, 3, skipValue);
  sqlite3_bind_text(stmt, 1, fileName.c_str(), -1, SQLITE_STATIC);
  if (sqlite3_step(stmt) == SQLITE_DONE)
  {
    std::cout << "Inserted values " << std::endl;
  }
  else
  {
    std::cout << "Insert Failed" << std::endl;
  }
  sqlite3_finalize(stmt);
}



