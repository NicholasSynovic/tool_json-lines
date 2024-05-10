#include "CLI/CLI.hpp"
#include "json/json.hpp"
#include "main.hpp"

#include <filesystem>
#include <indicators/indeterminate_progress_bar.hpp>
#include <indicators/progress_bar.hpp>
#include <sqlite3.h>

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

    return data;
}

int main(int argc, char **argv) {
    path jsonLinesPath;
    path sqliteDBPath;
    sqlite3* dbConn;

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

    path jsonLinesAbsolutePath = absolute(jsonLinesPath);
    path sqlite3DBAbsolutePath = absolute(sqliteDBPath);

    int sqlite3DBReturnCode = sqlite3_open(sqlite3DBAbsolutePath.c_str(), &dbConn);
    if(sqlite3DBReturnCode != SQLITE_OK)    {
        cout << string("Error opening ").append(sqlite3DBAbsolutePath);
        return 1;
    }

    vector<string> data = readFile(jsonLinesAbsolutePath);
    vector<json> jsonData = convertStringsToJSON(data);

    return 0;
}
