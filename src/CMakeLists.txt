add_executable(${CMAKE_PROJECT_NAME} main.cpp json/json.cpp)

target_link_libraries(
    ${CMAKE_PROJECT_NAME}
    PRIVATE CLI11
    PRIVATE nlohmann_json
    PRIVATE indicators
    )
