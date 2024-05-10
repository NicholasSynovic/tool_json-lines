#include <sqlite3.h>
#include <string>
#include <vector>

#ifndef SQLITE
#define SQLITE

// Function declaration
void createTables(sqlite3* dbConn);
sqlite3_stmt* _createSQLite3Statement(sqlite3* dbConn, std::string sql);
int writeData_JSON(sqlite3* dbConn, std::vector<std::string> jsonStrings);

#endif
