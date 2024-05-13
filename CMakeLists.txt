cmake_minimum_required(VERSION 3.25)
project(json-lines VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_CXX_FLAGS "-g -O3 -I. -std=c++20 -Wunknown-pragmas -fopenmp")

include(FetchContent)

# SQLite3
# find_package(SQLite3 REQUIRED)
# include_directories(${SQLITE3_INCLUDE_DIRS})

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
# fmtlib/fmt
FetchContent_Declare(
    fmt_proj
    QUIET
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 10.2.1
)
# SRombauts/SQLiteCpp
FetchContent_Declare(
    sqlite3_proj
    QUIET
    GIT_REPOSITORY https://github.com/SRombauts/SQLiteCpp.git
    GIT_TAG 3.3.1
)

FetchContent_MakeAvailable(
    cli11_proj
    json_proj
    indicators_proj
    fmt_proj
    sqlite3_proj
)

add_subdirectory(src)
