/*
 * test_recent_stops.cpp
 * Contains unit tests for the corrsponding recent_stops.cpp
 */
// #define CATCH_CONFIG_MAIN

// standard
#include <iostream>
// navigation
#include "../../../main/cpp/io/recent_stops.cpp"
// catch2
#include <catch2/catch_test_macros.hpp>

TEST_CASE ( "No action on shortest absolute path to target directory", "[collapse path]")
{
    std::string expectedPath = "/a/b/c/d/";
    std::string actualPath = expectedPath;

    collapse_entry_erase(actualPath);
    REQUIRE(expectedPath == actualPath);
    actualPath = "/a/b/c/d/";

    collapse_entry_erase(actualPath);
    REQUIRE(expectedPath == actualPath);
}

TEST_CASE ( "Remove trailing parent directory", "[collapse path]")
{
    std::string expectedPath = "/a/b/c/";
    std::string actualPath = "/a/b/c/d/../";

    collapse_entry_erase(actualPath);
    REQUIRE(expectedPath == actualPath);
}

TEST_CASE ( "Remove parent directory in path", "[collapse path]")
{
    std::string expectedPath = "/a/b/d/";
    std::string actualPath = "/a/b/c/../d/";

    collapse_entry_erase(actualPath);
    REQUIRE(expectedPath == actualPath);
}

TEST_CASE ( "Remove parent directory from start of path", "[collapse path]")
{
    std::string expectedPath = "/a/b/c/d/";
    std::string actualPath = "/../a/b/c/d/";

    collapse_entry_erase(actualPath);
    REQUIRE(expectedPath == actualPath);
}

TEST_CASE ( "Remove multiple parent directories from start of path", "[collapse path]")
{
    std::string expectedPath = "/c/d/";
    std::string actualPath = "/a/b/../../c/d/";

    collapse_entry_erase(actualPath);
    REQUIRE(expectedPath == actualPath);
}
