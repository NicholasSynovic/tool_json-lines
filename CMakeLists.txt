cmake_minimum_required(VERSION 3.25)
project(json-lines VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_CXX_FLAGS "-g -O3 -I. -std=c++20 -Wunknown-pragmas -fopenmp")

include(FetchContent)

# CLIUtils/CLI11
FetchContent_Declare(
    cli11_proj
    QUIET
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG v2.4.2
)
# nlohmann/json
FetchContent_Declare(
    json_proj
    QUIET
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)
# p-ranav/indicators
FetchContent_Declare(
    indicators_proj
    QUIET
    GIT_REPOSITORY https://github.com/p-ranav/indicators.git
    GIT_TAG v2.3
)
FetchContent_MakeAvailable(
    cli11_proj
    json_proj
    indicators_proj
    )

add_subdirectory(src)
