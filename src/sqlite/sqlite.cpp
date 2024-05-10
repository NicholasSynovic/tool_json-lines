#include <string>
#include <indicators/progress_bar.hpp>
#include <vector>
#include <sqlite3.h>
#include <iostream>

using namespace std;
using namespace indicators;

void createTables(sqlite3* dbConn)   {
    vector<string> sql = {
        "CREATE TABLE IF NOT EXISTS json (id INTEGER PRIMARY KEY, json_string TEXT);",
    };

    /* ProgressBar bar{ */
    /*     option::BarWidth{20}, */
    /*     option::Start{"["}, */
    /*     option::Fill{"="}, */
    /*     option::Lead{">"}, */
    /*     option::Remainder{" "}, */
    /*     option::End{"]"}, */
    /*     option::PostfixText{"Creating tables in SQLite3 database"}, */
    /*     option::ForegroundColor{Color::green}, */
    /*     option::FontStyles{std::vector<FontStyle>{FontStyle::bold}}, */
    /*     option::MaxProgress{sql.size()}, */
    /*     option::ShowElapsedTime{true}, */
    /*     option::ShowRemainingTime{true}, */
    /* }; */

    char* sqlite3ErrorMessage;
    for(int i = 0; i < sql.size(); i++) {
        cout << sql[i] << endl;
        int returnCode = sqlite3_exec(dbConn, sql[i].c_str(), nullptr, nullptr,
                                      &sqlite3ErrorMessage);
        if(returnCode != SQLITE_OK) {
            cout << "Error creating table" << sqlite3ErrorMessage << endl;
            sqlite3_free(sqlite3ErrorMessage);
        }
    }
}

void writeData(sqlite3* dbconn, vector<string> data) {
}
