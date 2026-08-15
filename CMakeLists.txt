# Project
cmake_minimum_required(VERSION 3.10)
project(Navigate)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()
include(FetchContent)

FetchContent_Declare(
    json
    GIT_REPOSITORY git@github.com:nlohmann/json.git
    GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(json)

find_library(HISTORY_LIB history)
find_library(READLINE_LIB readline)

# Compilation
add_executable(navigate ../src/main/cpp/main.cpp
                        ../src/main/cpp/util/binary_tree.cpp)
target_link_libraries(navigate PRIVATE nlohmann_json::nlohmann_json)

add_executable(recent-stops ../src/main/cpp/io/recent_stops.cpp)
target_link_libraries(recent-stops PRIVATE nlohmann_json::nlohmann_json ${HISTORY_LIB} ${READLINE_LIB})
