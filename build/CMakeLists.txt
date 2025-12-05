# Project
cmake_minimum_required(VERSION 3.10)
project(Navigate)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Packages
add_subdirectory(json)

# Compilation
add_executable(navigate ../src/navigate.cc)
target_link_libraries(navigate PRIVATE nlohmann_json::nlohmann_json)
