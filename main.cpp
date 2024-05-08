#include "CLI/CLI.hpp"
#include <iostream>

using namespace std;

int main(int argc, char **argv) {
    CLI::App app{"Hello World Example"};

    string text = "Hello World!";
    app.add_option("-i", text, "Text to print")    ->capture_default_str();

    CLI11_PARSE(app, argc, argv);

    cout << text << "\n";
    return 0;
}
