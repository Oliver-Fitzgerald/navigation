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
# Compilation
add_executable(navigate ../src/navigate.cc)
target_link_libraries(navigate PRIVATE nlohmann_json::nlohmann_json)
