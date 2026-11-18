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

TEST_CASE ( "No action on shortest absolute path to target directory", "[collapse '..' in path]")
{
    std::string expectedPath = "/a/b/c/d/";
    std::string actualPath = expectedPath;

    collapse_all(actualPath);
    REQUIRE(expectedPath == actualPath);
    actualPath = "/a/b/c/d/";

    collapse_all(actualPath);
    REQUIRE(expectedPath == actualPath);
}

TEST_CASE ( "Remove trailing parent directory", "[collapse '..' in path]")
{
    std::string expectedPath = "/a/b/c/";
    std::string actualPath = "/a/b/c/d/../";

    collapse_all(actualPath);
    REQUIRE(expectedPath == actualPath);
}

TEST_CASE ( "Remove parent directory in path", "[collapse '..' in path]")
{
    std::string expectedPath = "/a/b/d/";
    std::string actualPath = "/a/b/c/../d/";

    collapse_all(actualPath);
    REQUIRE(expectedPath == actualPath);
}

TEST_CASE ( "Remove parent directory from start of path", "[collapse '..' in path]")
{
    std::string expectedPath = "/a/b/c/d/";
    std::string actualPath = "/../a/b/c/d/";

    collapse_all(actualPath);
    REQUIRE(expectedPath == actualPath);
}

TEST_CASE ( "Remove multiple parent directories from start of path", "[collapse '..' in path]")
{
    std::string expectedPath = "/c/d/";
    std::string actualPath = "/a/b/../../c/d/";

    collapse_all(actualPath);
    REQUIRE(expectedPath == actualPath);
}

TEST_CASE ( "Remove single curent directory from start of path", "[collapse '.' in path]")
{
    std::string expectedPath = "/a/b/c/d/";
    std::string actualPath = "/./a/b/c/d/";

    collapse_all(actualPath);
    REQUIRE(expectedPath == actualPath);
}

TEST_CASE ( "Remove single curent directory from middle of path", "[collapse '.' in path]")
{
    std::string expectedPath = "/a/b/c/d/";
    std::string actualPath = "/a/b/./c/d/";

    collapse_all(actualPath);
    REQUIRE(expectedPath == actualPath);
}

TEST_CASE ( "Remove single curent directory from end of path", "[collapse '.' in path]")
{
    std::string expectedPath = "/a/b/c/d/";
    std::string actualPath = "/a/b/c/d/./";

    collapse_all(actualPath);
    REQUIRE(expectedPath == actualPath);
}

TEST_CASE ( "Remove multiple curent directory from middle of path", "[collapse '.' in path]")
{
    std::string expectedPath = "/a/b/c/d/";
    std::string actualPath = "/a/b/././c/d/";

    collapse_all(actualPath);
    REQUIRE(expectedPath == actualPath);
}

TEST_CASE ( "Does not remove dot directories in path", "[collapse '.' in path]")
{
    std::string expectedPath = "/a/.b/.c/d/";
    std::string actualPath = "/a/.b/.c/d/";

    collapse_all(actualPath);
    REQUIRE(expectedPath == actualPath);
}
