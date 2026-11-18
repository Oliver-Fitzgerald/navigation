/*
 * recent_stop.cpp
 * executed at the end of a bash session to update a record of the most
 * frequently visited directories
 */

// standard
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <filesystem>
#include <iterator>
// gnu
#include <readline/history.h>
// spdlog
#include "spdlog/spdlog.h"
// navigation
#include "configuration.h"

void read_target_directories(std::vector<std::string>& commands);
void resolve_absolute_path(std::vector<std::string>& directories);
void collapse_all(std::string& path);
void collapse(std::basic_string<char>::iterator begin, std::basic_string<char>::iterator end);
void remove_null_chars(std::string& path);

std::string USER = std::getenv("USER"); std::string HISTORY_PATH = "/home/" + USER + "/.history";
std::string CONFIG_PATH = "/home/" + USER + "/projects/personal/navigate/src/main/resources/recent_directories.json";

int main(int argc, char** argv) {
    spdlog::debug("Executing navigation clean up");

    // initialization
    int error;
    using_history();

    error = read_history(HISTORY_PATH.c_str());
    if (error - error) {
        spdlog::debug("error reading history: {0}", error);
        return error;
    }

    // read cd commands from history
    std::vector<std::string> directories;
    read_target_directories(directories);

    // get absolute path (if not explicit in command)
    resolve_absolute_path(directories); 

    // update directory frequency of visits TODO

    return 0;
}

/**
 * read_target_directories
 * Reads in the target directory of all executed cd commands in this session
 *
 * @param commands The vector to store executed commands
 */
void read_target_directories(std::vector<std::string>& commands)
{
    while (!history_search_prefix("cd",1))
    {
        HIST_ENTRY* entry = current_history();
        commands.push_back(std::string(entry->line));

        next_history();
    }

    // extract target directory from entries
    for (std::string& entry : commands)
    {
        spdlog::debug("raw entry: {0}",entry);
        if (entry == "cd") // TODO trim white space
            entry = "/home/" + USER + "/";
        else
            entry = entry.substr(3);

        if (entry.front() == '~')
            entry.replace(0,2,std::string("/home/" + USER + "/"));
        spdlog::debug("read entry: {0}",entry);
    }
}

/**
 * resolve_absolute_path
 * Resolves the absolute path for target directories for which only a relative
 * path was used.
 *
 * Note can be replaced with: https://en.cppreference.com/cpp/filesystem/path/lexically_normal
 * but I spent to much time working on it before I realized so it stays
 * sunk cost fallacy: https://en.wikipedia.org/wiki/Sunk_cost
 *
 * @param directories All target directories of the cd command
 */
void resolve_absolute_path(std::vector<std::string>& directories) {
    namespace fs = std::filesystem;

    std::string entrypoint(std::getenv("SHELL_ENTRYPOINT"));
    if (entrypoint.back() != '/')
        entrypoint.push_back('/');
    std::error_code errorCode;

    for (std::string& directory: directories)
    {
        spdlog::info("resolving absolute path of directory: {0}", directory);
        if (directory.front() != '/') {
            directory = entrypoint + directory;
        }

        //  validate absolute path exists
        fs::path directoryPath(directory);
        spdlog::debug("directory: {0}", directory);
        fs::directory_entry directory_entry(directoryPath, errorCode);
        spdlog::debug("error value: {0}", errorCode.value());

        if (errorCode.value() != 0) {
            spdlog::warn("The absolute path of the directory ({0}) cannot be resolved", directory);
            continue;
        }

        entrypoint = directory;

        // collapse all '..' && '.' entries in path
        collapse_all(directory);
        spdlog::info("absolute path resolved to: {0}", directory);
    }
}

/**
 * collapse_all
 * Collapses '..' and '.' entries from a path to get the shortest absolute path
 * to a target directory
 *
 * @param path A valid absolute path entry to be collapsed
 */
void collapse_all(std::string& path) {

    std::basic_string<char>::iterator leadIterator = path.begin();
    std::vector<std::basic_string<char>::iterator> directoryEntries;

    directoryEntries.push_back(leadIterator);

    char lastChar[] = {'\0', '\0'};
    int hits = 0;

    while (leadIterator != path.end())
    {
        spdlog::debug("path: {0}", path);
        spdlog::debug("char: {0}", *leadIterator);

        if (*leadIterator == '/') {

            if (lastChar[0] == '.' && lastChar[1] == '.') {
                spdlog::debug("collapse back");
                collapse(directoryEntries[directoryEntries.size() - 2], leadIterator);
                directoryEntries.pop_back();
                hits = 0;
            } else if (lastChar[0] == '/' && lastChar[1] == '.') {

                spdlog::debug("collapse current");
                collapse(directoryEntries[directoryEntries.size() - 1], leadIterator);
                hits = 0;

            } else { 
                hits++;
                directoryEntries.push_back(leadIterator);
            }
        }

        lastChar[0] = lastChar[1];
        lastChar[1] = *leadIterator;
        leadIterator++;
    }

    for (char c : path) {
        spdlog::debug("\'{0}\'", c);
    }
    spdlog::debug("\n");
    remove_null_chars(path);
    for (char c : path) {
        spdlog::debug("\'{0}\'", c);
    }
}


/**
 * collapse
 * DESCRIPTION
 * RETURN
 */
void collapse(std::basic_string<char>::iterator begin, std::basic_string<char>::iterator end) {
    while (begin != end)
    {
        spdlog::debug("poping off char: {0}", *begin);
        *begin = '\0';
        ++begin;
    }
}

/**
 * remove_null_chars
 * DESCRIPTION
 * RETURN
 */
void remove_null_chars(std::string& path) {

    std::basic_string<char>::iterator read = path.begin();
    std::basic_string<char>::iterator write = path.begin();
    int index = 0;

    while (read != path.end())
    {
        if (*read != '\0') {
            index++;
            *write = *read;
            if (write != read)
                *read = '\0';
            write++;
        }
        read++;
    }

    spdlog::debug("{0}",index);
    if (write != path.end())
        path.resize(index);
}


/**
 * update_recent_directories
 * increments existing directory entries if present otherwise appends
 * a new entry to recent_directories.json
 */
void update_recent_directories() {
    try {
        std::ofstream file(CONFIG_PATH);
        for (int i = 0; i < 10; i++)
            file << i;
    } catch (std::exception& exception) {
        spdlog::debug(exception.what());
    }
}
