#include "CLI/CLI.hpp"
#include "fmt/format.h"
#include "json/json.hpp"
#include "main.hpp"
#include "indicators/indeterminate_progress_bar.hpp"
#include "indicators/progress_bar.hpp"
#include "indicators/termcolor.hpp"
#include "SQLiteCpp/SQLiteCpp.h"

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace std::literals;
using namespace indicators;
using namespace SQLite;
using filesystem::absolute;
using filesystem::path;
using fmt::format;
using nlohmann::json;

int BAR_WIDTH = 20;

ProgressBar PROGRESS_BAR {
    option::BarWidth{BAR_WIDTH},
    option::Start{"["},
    option::Fill{"="},
    option::Lead{">"},
    option::Remainder{" "},
    option::End{"]"},
    option::PostfixText{"CHANGE ME"},
    option::ForegroundColor{Color::green},
    option::FontStyles{std::vector<FontStyle>{FontStyle::bold}},
    option::MaxProgress{100},
    option::ShowElapsedTime{true},
    option::ShowRemainingTime{true},
};

IndeterminateProgressBar I_PROGRESS_BAR {
    option::BarWidth{BAR_WIDTH},
    option::Start{"["},
    option::Fill{"."},
    option::Lead{"<=>"},
    option::End{"]"},
    option::PostfixText{"CHANGE ME"},
    option::ForegroundColor{Color::green},
    option::FontStyles{std::vector<FontStyle>{FontStyle::bold}},
};

bool createDBTables(path dbPath)    {
    vector<string> sqlQueries {
        "CREATE TABLE IF NOT EXISTS test(id INTEGER PRIMARY KEY, test_string TEXT);",
    };

    PROGRESS_BAR.set_option(option::PostfixText{"Creating database tables"});
    PROGRESS_BAR.set_option(option::MaxProgress{sqlQueries.size()});
    PROGRESS_BAR.set_progress(0);

    Database dbConn = Database(dbPath, OPEN_READWRITE|OPEN_CREATE);

    try {
        for(int i = 0; i < sqlQueries.size(); i++) {
            Statement sqlQuery = Statement(dbConn, sqlQueries[i]);
            dbConn.exec(sqlQueries[i]);
            PROGRESS_BAR.tick();
        }
    }
    catch(exception& e) {
        cout << termcolor::reset << endl;
        cout << "Exception caught while creating tables: " << e.what() << endl;
        return false;
    }

    PROGRESS_BAR.set_option(option::PostfixText{"Created database tables"});
    cout << termcolor::reset << endl;
    return true;
}

vector<string> readFile(path filepath)  {
    string line;
    ifstream jsonFile;

    vector<string> data = {};

    I_PROGRESS_BAR.set_option(option::PostfixText{format("Reading {}", filepath.filename().string())});

    jsonFile.open(filepath);
    if(jsonFile.is_open())  {
        while(getline(jsonFile, line))  {
            data.insert(data.end(), line);
            I_PROGRESS_BAR.tick();
        };
        jsonFile.close();
    };

    I_PROGRESS_BAR.set_option(option::PostfixText{format("Read {}", filepath.filename().string())});

    cout << termcolor::reset << endl;
    return data;
}

string buildSQLTransaction(vector<string> data) {
    string sql = string("BEGIN TRANSACTION;\n");

    PROGRESS_BAR.set_option(option::PostfixText{"Building SQL transaction"});
    PROGRESS_BAR.set_option(option::MaxProgress{data.size()});
    PROGRESS_BAR.set_progress(0);

    for(int i = 0; i < data.size(); i++)    {
        string templateString =
            format("INSERT INTO test(test_string) VALUES ({});\n", "\"fred\"");

        sql.append(templateString);
        PROGRESS_BAR.tick();
    }
    sql.append("COMMIT TRANSACTION;");

    PROGRESS_BAR.set_option(option::PostfixText{format("Built SQL transaction")});
    PROGRESS_BAR.mark_as_completed();


    cout << termcolor::reset << endl;
    return sql;
}

bool evaluteSQL(path dbPath, string sql)    {
    Database dbConn = Database(dbPath, OPEN_READWRITE|OPEN_CREATE);

    try {
        Statement sqlQuery = Statement(dbConn, sql);
        cout << sqlQuery.getQuery() << endl;
        dbConn.exec(sql);

    }
    catch(exception& e) {
        cout << "Exception caught while executing SQL: " << e.what() << endl;
        return false;
    }
    cout << "Executed SQL" << endl;
    return true;
}

int main(int argc, char **argv) {
    // Allocate variables
    path jsonLinesPath;
    path sqliteDBPath;

    // Command line arg parsing
    CLI::App app{"JSON Lines to SQLite3 Database"};
    app.add_option("-i,--input", jsonLinesPath,
                   "Path to JSON Lines file")
       ->required()
       ->check(CLI::ExistingFile);

    app.add_option("-o,--output", sqliteDBPath,
                   "Path to SQLite3 database to store output")
       ->required()
       ->check(CLI::TypeValidator<path>());

    CLI11_PARSE(app, argc, argv);

    // Make absolute paths
    path jsonLinesAbsolutePath = absolute(jsonLinesPath);
    path sqlite3DBAbsolutePath = absolute(sqliteDBPath);

    // Open SQLite3 database and create tables
    createDBTables(sqlite3DBAbsolutePath);

    // Load JSON Lines data into memory
    vector<string> data = readFile(jsonLinesAbsolutePath);

    // Build SQL transaction query
    string sqlTransactionQuery = buildSQLTransaction(data);

    // Write data to database
    evaluteSQL(sqlite3DBAbsolutePath, sqlTransactionQuery);

    return 0;
}
