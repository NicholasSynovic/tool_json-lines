#include "CLI/CLI.hpp"
#include <filesystem>
#include "nlohmann/json.hpp"
#include <indicators/progress_bar.hpp>
#include <indicators/indeterminate_progress_bar.hpp>
#include "main.hpp"
#include "common/common.hpp"

using namespace indicators;
using namespace std;
using filesystem::path;
using filesystem::absolute;
using nlohmann::json;

vector<string> readFile(path file)  {
    string line;
    ifstream jsonFile;

    vector<string> data = {};

    IndeterminateProgressBar ibar{
        option::BarWidth{MAX_BAR_SIZE},
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

vector<json> convertToJson(vector<string> jsonStrings)   {
    int jsonStringsSize = jsonStrings.size();

    ProgressBar bar{
        option::BarWidth{MAX_BAR_SIZE},
        option::Start{"["},
        option::Fill{"="},
        option::Lead{">"},
        option::Remainder{" "},
        option::End{"]"},
        option::PostfixText{"Converting strings to JSON"},
        option::ForegroundColor{Color::green},
        option::FontStyles{std::vector<FontStyle>{FontStyle::bold}},
        option::MaxProgress{jsonStringsSize},
        option::ShowElapsedTime{true},
        option::ShowRemainingTime{true},
    };

    vector<json> data(jsonStringsSize);

    int counter = 0;
    /* #pragma omp parallel for */
    for(int i = 0; i < jsonStringsSize; i++) {
        json jsonData = json::parse(jsonStrings[i]);
        data[i] = jsonData;
        bar.tick();
    }

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

    vector<json> jsonData = convertToJson(data);

    return 0;
}
