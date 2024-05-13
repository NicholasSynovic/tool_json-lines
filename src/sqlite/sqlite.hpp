#include <sqlite3.h>
#include <string>
#include <vector>
#include "SQLiteCpp/SQLiteCpp.h"
#include <filesystem>
#include <any>

#ifndef SQLITE
#define SQLITE

// Function declaration
SQLite::Database openDatabase(std::filesystem::path path);
SQLite::Transaction createTransaction(SQLite::Database dbConn, std::vector<std::any> data);
bool submitTransaction(SQLite::Database, SQLite::Transaction transaction);
bool createDBTables(SQLite::Database db);

/* void createTables(sqlite3* dbConn); */
/* sqlite3_stmt* _createSQLite3Statement(sqlite3* dbConn, std::string sql); */
/* int writeData_JSON(sqlite3* dbConn, std::vector<std::string> jsonStrings); */
/* std::string _createSQLQuery(sqlite3* dbConn, std::vector<std::string> data); */
/* int evaluteSQL(sqlite3* dbConn, std::string sql); */
#endif
