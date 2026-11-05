/*
 * recent_stop.cpp
 * Designed to be executed at the end of a bash session to
 * update a record with the directories visited in this session
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

void read_target_directories(std::vector<std::string>& commands);
void resolve_absolute_path(std::vector<std::string>& directories);
void collapse_entry(std::string& path);
void collapse(std::basic_string<char>::iterator begin, std::basic_string<char>::iterator end);
void remove_empty_chars(std::string& path);

std::string USER = std::getenv("USER"); std::string HISTORY_PATH = "/home/" + USER + "/.history";
std::string CONFIG_PATH = "/home/" + USER + "/projects/personal/navigate/src/main/resources/recent_directories.json";

// int main(int argc, char** argv) {
//     std::cout << "Executing navigation clean up\n";
//
//     // initialization
//     int error;
//     using_history();
//
//     error = read_history(HISTORY_PATH.c_str());
//     if (error - error) {
//         std::cout << "error reading history: " << error << "\n";
//         return error;
//     }
//
//     // read cd commands from history
//     std::vector<std::string> directories;
//     read_target_directories(directories);
//
//     // get absolute path (if not explicit in command) TODO
//     resolve_absolute_path(directories); 
//
//     // update directory frequency of visits TODO
//     return 0;
// }

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
        // std::cout << "raw entry: " << entry << "\n";
        if (entry == "cd") // TODO trim white space
            entry = "/home/" + USER + "/";
        else
            entry = entry.substr(3);

        if (entry.front() == '~')
            entry.replace(0,2,std::string("/home/" + USER + "/"));
        // std::cout << "read entry: " << entry << "\n";
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

    for (std::string& entry: directories)
    {
        //  validate absolute path exists
        if (entry.front() == '/') {

            fs::path directoryPath(entry);
            fs::directory_entry directory_entry(directoryPath, errorCode);

        // construct absolute path for relative paths from entrypoint
        } else {

            entry = entrypoint + entry;
            fs::path directoryPath(entry);
            fs::directory_entry directory_entry(directoryPath, errorCode);
        }

        std::cout << "entry: " << entry << "\n";
        std::cout << "error value: " << errorCode.value() << "\n";
        std::cout << "error value: " << errorCode.value() << "\n";

        if (errorCode.value() != 0) continue;
        entrypoint = entry;

        // collapse '..' 
        collapse_entry(entry);
        std::cout << "collapsed entry: " << entry << "\n";
    }
}

/**
 * collapse_entry
 * Collapses '..' and '.' entries from a path to get the shortest absolute path
 * to a target directory
 *
 * @param path A valid absolute path entry to be collapsed
 */
void collapse_entry(std::string& path) {

    // duplicated logic with resolve_absolute_path but required for unit tests
    if (path.back() != '/')
        path.push_back('/');

    std::basic_string<char>::iterator character = path.begin();
    std::vector<std::basic_string<char>::iterator> checkpoint = { path.begin() }; 

    char lastChar[2] = {'\0', '\0'};
    while (character != path.end())
    {
        std::cout << "working on char (" << *character << ")\n";
        // ../
        if (*character == '/' && lastChar[0] == '.' && lastChar[1] == '.') {
            std::cout << "collapsing back directory\n";
            // collapse
            checkpoint.pop_back();
            collapse(checkpoint.back(), character);
            checkpoint.pop_back();
        }
        else if (*character != '.' && lastChar[0] == '.' && (lastChar[1] != '.' || lastChar[1] != '\0')) {
            std::cout << "clearing single .\n";
            // clear './
            collapse(checkpoint.back(), character);
            checkpoint.pop_back();
        }

        if (*character == '/') {
            std::cout << "moving pointer to next directory entry\n";
            checkpoint.push_back(character);
        }
        lastChar[1] = lastChar[0];
        lastChar[0] = *character;
        ++character;
    }
}

/**
 * collapse_entry_erase
 */
void collapse_entry_erase(std::string& path) {

    std::basic_string<char>::iterator leadIterator = path.begin();
    std::vector<std::basic_string<char>::iterator> directoryEntries;

    directoryEntries.push_back(leadIterator);

    char lastChar[] = {'\0', '\0'};
    int hits = 0;

    while (leadIterator != path.end())
    {
        std::cout << "path: " << path << "\n";
        std::cout << "char: " << *leadIterator << "\n";

        if (*leadIterator == '/') {

            if (lastChar[0] == '.' && lastChar[1] == '.') {
                std::cout << "collapse\n";
                collapse(directoryEntries[directoryEntries.size() - 2], leadIterator);
                directoryEntries.pop_back();
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
        std::cout << "\'" << c << "\'";
    }
    std::cout << "\n";
    remove_empty_chars(path);
    for (char c : path) {
        std::cout << "\'" << c << "\'";
    }
    std::cout << "\n";
}


/**
 * collapse
 * DESCRIPTION
 * RETURN
 */
void collapse(std::basic_string<char>::iterator begin, std::basic_string<char>::iterator end) {
    while (begin != end)
    {
        std::cout << "poping of char: " << *begin << "\n";
        *begin = '\0';
        ++begin;
    }
}

/**
 * remove_empty_chars
 * DESCRIPTION
 * RETURN
 */
void remove_empty_chars(std::string& path) {

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

    std::cout << index << "\n";
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
        std::cout << "three";
    } catch (std::exception& exception) {
        std::cout << "1\n";
        std::cout << exception.what();
    }
    std::cout << "2\n";
}
