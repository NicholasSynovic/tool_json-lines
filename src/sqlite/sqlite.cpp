#include <string>
#include "fmt/format.h"
#include <indicators/progress_bar.hpp>
#include <indicators/cursor_control.hpp>
#include <indicators/termcolor.hpp>
#include <vector>
#include <sqlite3.h>
#include <iostream>

using namespace std;
using namespace indicators;
using namespace fmt;

void createTables(sqlite3* dbConn)   {
    vector<string> sql = {
        "CREATE TABLE IF NOT EXISTS json (id INTEGER PRIMARY KEY, json_string TEXT);",
    };

    char* sqlite3ErrorMessage;
    for(int i = 0; i < sql.size(); i++) {
        cout << "SQL: " << sql[i] << endl;
        int returnCode = sqlite3_exec(dbConn, sql[i].c_str(), nullptr, nullptr,
                                      &sqlite3ErrorMessage);
        if(returnCode != SQLITE_OK) {
            cerr << "Error creating table: " << sqlite3ErrorMessage << endl;
            sqlite3_free(sqlite3ErrorMessage);
        }
    }
}

string _createSQLQuery(vector<string> data) {
    string templateString = string();
    return templateString;
}

sqlite3_stmt* _createSQLite3Statement(sqlite3* dbConn, string sql)   {
    sqlite3_stmt* sqlite3Statement;
    int sqlite3StatementReturnCode = sqlite3_prepare_v2(dbConn, sql.c_str(), -1,
                                     &sqlite3Statement, nullptr);
    if(sqlite3StatementReturnCode != SQLITE_OK) {
        cerr << "Error preparing statement: " << sqlite3_errmsg(dbConn) << endl;
        exit(1);
    }
    else {
        return sqlite3Statement;
    }
}

int writeData_JSON(sqlite3* dbConn, vector<string> jsonStrings) {
    string sql = "INSERT INTO json (json_string) VALUES (?)";
    sqlite3_stmt* sqlite3Statement = _createSQLite3Statement(dbConn, sql);

    ProgressBar bar{
        option::BarWidth{20},
        option::Start{"["},
        option::Fill{"="},
        option::Lead{">"},
        option::Remainder{" "},
        option::End{"]"},
        option::PostfixText{"Writing data to the json table"},
        option::ForegroundColor{Color::green},
        option::FontStyles{std::vector<FontStyle>{FontStyle::bold}},
        option::MaxProgress{jsonStrings.size()},
        option::ShowElapsedTime{true},
        option::ShowRemainingTime{true},
    };

    for(int i = 0; i < jsonStrings.size(); i++) {
        sqlite3_bind_text(sqlite3Statement, 1, jsonStrings[i].c_str(), -1,
                          SQLITE_STATIC);

        int sqlite3StepReturnCode = sqlite3_step(sqlite3Statement);
        if(sqlite3StepReturnCode != SQLITE_DONE)  {
            cerr << "Error inserting data: " << i << endl;
            return 1;
        }
        else {
            sqlite3_reset(sqlite3Statement);
            bar.tick();
        }
    }
    sqlite3_finalize(sqlite3Statement);
    cout << termcolor::reset << endl;
    return 0;
}
