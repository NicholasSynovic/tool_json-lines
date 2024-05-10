#include "CLI/CLI.hpp"
#include "json/json.hpp"
#include "main.hpp"

#include <filesystem>
#include <indicators/indeterminate_progress_bar.hpp>
#include <indicators/progress_bar.hpp>

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
    CLI::App app{"Print JSON Lines"};

    path jsonLinesPath;
    app.add_option("-i,--input", jsonLinesPath,
                   "Path to file")
       ->required()
       ->check(CLI::ExistingFile);

    CLI11_PARSE(app, argc, argv);

    path jsonLinesAbsolutePath = absolute(jsonLinesPath);

    vector<string> data = readFile(jsonLinesAbsolutePath);

    vector<json> jsonData = convertStringsToJSON(data);

    return 0;
}
