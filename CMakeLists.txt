cmake_minimum_required(VERSION 3.25)
project(json-lines VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

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

FetchContent_MakeAvailable(
    cli11_proj
    json_proj
    )

add_executable(${CMAKE_PROJECT_NAME} main.cpp)

target_link_libraries(
    ${CMAKE_PROJECT_NAME}
    CLI11::CLI11
    nlohmann_json::nlohmann_json
    )
