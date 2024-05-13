add_executable(${CMAKE_PROJECT_NAME} main.cpp json/json.cpp sqlite/sqlite.cpp)

target_link_libraries(
    ${CMAKE_PROJECT_NAME}
    PRIVATE SQLiteCpp
    PRIVATE CLI11
    PRIVATE nlohmann_json
    PRIVATE indicators
    PRIVATE fmt
    )
