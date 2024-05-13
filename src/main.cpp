#include "CLI/CLI.hpp"
#include "json/json.hpp"
#include "sqlite/sqlite.hpp"
#include "main.hpp"

#include <filesystem>
#include <indicators/cursor_control.hpp>
#include <indicators/indeterminate_progress_bar.hpp>
#include <indicators/progress_bar.hpp>
#include <sqlite3.h>
#include <indicators/termcolor.hpp>

using namespace indicators;
using namespace std;

using filesystem::absolute;
using filesystem::path;
using nlohmann::json;

vector<string> readFile(path file)  {
    string line;
    ifstream jsonFile;

    vector<string> data = {};

    IndeterminateProgressBar ibar{
        option::BarWidth{20},
        option::Start{"["},
        option::Fill{"."},
        option::Lead{"<=>"},
        option::End{"]"},
        option::PostfixText{string("Reading ").append(file.filename())},
        option::ForegroundColor{Color::green},
        option::FontStyles{std::vector<FontStyle>{FontStyle::bold}},
    };

    jsonFile.open(file);
    if(jsonFile.is_open())  {
        while(getline(jsonFile, line))  {
            data.insert(data.end(), line);
            ibar.tick();
        };
        jsonFile.close();
    };

    cout << termcolor::reset << endl;
    return data;
}

int main(int argc, char **argv) {
    // Allocate variables
    path jsonLinesPath;
    path sqliteDBPath;
    sqlite3* dbConn;

    // Command line arg parsing
    CLI::App app{"JSON Lines to SQLite3 Database"};
    app.add_option("-i,--input", jsonLinesPath,
                   "Path to JSON Lines file")
       ->required()
       ->check(CLI::ExistingFile);

    app.add_option("-o,--output", sqliteDBPath,
                   "Path to SQLite3 database to store output")
       ->required()
       ->check(CLI::NonexistentPath);

    CLI11_PARSE(app, argc, argv);

    // Make absolute paths
    path jsonLinesAbsolutePath = absolute(jsonLinesPath);
    path sqlite3DBAbsolutePath = absolute(sqliteDBPath);

    // Open SQLite3 database
    int sqlite3DBReturnCode = sqlite3_open(sqlite3DBAbsolutePath.c_str(), &dbConn);
    if(sqlite3DBReturnCode != SQLITE_OK)    {
        cerr << string("Error opening ").append(sqlite3DBAbsolutePath);
        return 1;
    }
    else {
        cout << "Opened " << sqlite3DBAbsolutePath << endl;
    }

    // Create SQLite3 tables if they do not exist
    createTables(dbConn);

    // Load JSON data into memory
    vector<string> data = readFile(jsonLinesAbsolutePath);
    cout << "Read file " << jsonLinesAbsolutePath << endl;

    /* vector<json> jsonData = convertStringsToJSON(data); */
    /* cout << "Converted JSON strings to JSON objects" << endl; */

    // Write data to database
    string sql = _createSQLQuery(dbConn, data);
    evaluteSQL(dbConn, sql);
    /* writeData_JSON(dbConn, data); */
    /* cout << "Wrote data to " << sqlite3DBAbsolutePath << endl; */

    // Close database
    sqlite3_close(dbConn);

    return 0;
}
