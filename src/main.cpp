#include "CLI/CLI.hpp"
#include <filesystem>
#include "nlohmann/json.hpp"
#include <indicators/progress_bar.hpp>

using namespace indicators;
using namespace std;
using filesystem::path;
using filesystem::absolute;
using nlohmann::json;

vector<string> readFile(path file)  {
    string line;
    ifstream jsonFile;

    vector<string> data = {};

    jsonFile.open(file);
    if(jsonFile.is_open())  {
        while(getline(jsonFile, line))  {
            data.insert(data.end(), line);
        };
        jsonFile.close();
    };

    return data;
}

vector<json> convertToJson(vector<string> jsonStrings)   {
    int jsonStringsSize = jsonStrings.size();

    ProgressBar bar{
        option::BarWidth{10},
        option::Start{"["},
        option::Fill{"="},
        option::Lead{">"},
        option::Remainder{" "},
        option::End{"]"},
        option::PostfixText{"Converting string to JSON"},
        option::ForegroundColor{Color::green},
        option::FontStyles{std::vector<FontStyle>{FontStyle::bold}},
        option::MaxProgress{jsonStringsSize},
    };

    vector<json> data(jsonStringsSize);

    int counter = 0;
    #pragma omp parallel for
    for(int i = 0; i < jsonStringsSize; i++) {
        json jsonData = json::parse(jsonStrings[i]);
        data[i] = jsonData;

        #pragma omp critical
        {
            bar.tick();
        }
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
