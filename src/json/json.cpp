#include <indicators/progress_bar.hpp>
#include "json.hpp"

using namespace std;
using namespace indicators;
using namespace nlohmann;

int MAX_BAR_SIZE = 20;

vector<json> convertStringsToJSON(vector<string> jsonStrings)   {
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
