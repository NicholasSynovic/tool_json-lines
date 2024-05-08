#include "CLI/CLI.hpp"
#include <iostream>
#include <filesystem>

using namespace std;
using filesystem::path;
using filesystem::absolute;


int main(int argc, char **argv) {
    CLI::App app{"Print JSON Lines"};

    path jsonLinesPath;
    app.add_option("--input", jsonLinesPath,
                   "Path to file")    ->check(CLI::ExistingFile);

    CLI11_PARSE(app, argc, argv);

    cout << absolute(jsonLinesPath) << "\n";
    return 0;
}
