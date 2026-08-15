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
#include <regex>
// gnu
#include <readline/history.h>

static const char* HISTORY_PATH = "/home/ollie/.bash_history";
static const char* CONFIG_PATH = "/home/ollie/projects/personal/navigation/src/main/resources/recent_directories.json";

int main(int argc, char** argv) {
    // initialization
    int error;
    using_history();

    error = read_history(HISTORY_PATH);
    if (error - error) {
        std::cout << "error reading history: " << error << "\n";
        return error;
    }

    // read cd commands from history
    std::vector<std::string> cdCommands;
    while (!history_search_prefix("cd ",1))
    {
        HIST_ENTRY* entry = current_history();
        cdCommands.push_back(std::string(entry->line));

        next_history();
    }

    // strip cd from entries
    for (std::string& entry : cdCommands)
        entry = entry.substr(3);

    // replace '..' entries
    std::remove_if(cdCommands.begin(), cdCommands.end(),
        [](std::string entry) {
            return entry.empty() || entry.back() != '/' || entry.front() != ' ' ? true : false;
        }),cdCommands.end());

    for (std::string& entry : cdCommands)
        std::cout << "'" << entry << "'\n"; = entry.substr(3);

    return 0;
    // // filter invalid entries
    // // ||  
    // cdCommands.erase(std::remove_if(cdCommands.begin(), cdCommands.end(),
    //                  [](std::string entry) {
    //                     return entry.empty() || entry.back() != '/' || entry.front() != ' ' ? true : false;
    //                 }), cdCommands.end());
    //
    // for (std::string entry : cdCommands)
    //     std::cout << "\"" << entry << "\"\n";
    // return 0;
    //
    //
    // // remove invalid instances
    // cdCommands.erase(std::remove_if(cdCommands.begin(), cdCommands.end(),
    //                  [](std::string entry) {
    //
    //                     if (entry.back() != '/' 
    //                      || entry.size() <= 6)
    //                      // || entry.substr(entry.size() - 3, entry.size()) == std::string("../"))
    //                         return false;
    //                     else 
    //                         return true;
    //                  }), cdCommands.end());
    //
    // // get target directory
    // for (std::string entry : cdCommands)
    // {
    //     // entry.pop_back();
    //     std::cout << entry << "\n";
    //     std::cout << entry.substr(entry.size() - 3, entry.size()) << "\n";
    //     // entry = entry.substr(entry.find_last_of('/'), entry.size()); 
    // }
    //
    // // update directory frequency of visits
    // return 0;
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
