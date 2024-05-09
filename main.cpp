#include "CLI/CLI.hpp"
#include <filesystem>
#include "nlohmann/json.hpp"

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
    vector<json> data = {};

    /* #pragma omp parallel for */
    for(int i = 0; i < jsonStrings.size(); i++) {
        data.insert(data.end(), json::parse(jsonStrings[i]));
        cout << i << "\n";
    }

    return data;
}


int main(int argc, char **argv) {
    CLI::App app{"Print JSON Lines"};

    path jsonLinesPath;
    app.add_option("--input", jsonLinesPath,
                   "Path to file")
       ->required()
       ->check(CLI::ExistingFile);

    CLI11_PARSE(app, argc, argv);

    path jsonLinesAbsolutePath = absolute(jsonLinesPath);

    vector<string> data = readFile(jsonLinesAbsolutePath);
    cout << "Read file" << "\n";

    vector<json> jsonData = convertToJson(data);

    for(int i = 0; i < jsonData.size(); i++)    {
        cout << jsonData[i] << "\n";
    };

    return 0;
}
