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

void read_target_directories(std::vector<std::string>& commands);
void resolve_absolute_path(std::vector<std::string>& directories);

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
    std::vector<std::string> directories;
    read_target_directories(directories);

    // get absolute path (if not explicit in command) TODO
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
    while (!history_search_prefix("cd ",1))
    {
        HIST_ENTRY* entry = current_history();
        commands.push_back(std::string(entry->line));

        next_history();
    }

    // strip 'cd ' from entries
    for (std::string& entry : commands)
        entry = entry.substr(3);

    // read in target directories
    commands.erase(std::remove_if(commands.begin(),commands.end(),
    [](std::string& entry) {
        if (entry.empty() || entry.back() != '/' || entry == "../")
        {
            return true;
        }
        else if (entry.front() != '~' && entry.front() != '/') 
        {
            entry.pop_back();
            if (entry.find('/') != -1)
                entry = entry.substr(entry.find_last_of('/') + 1, entry.size() - 1);
        }
        return false;
    }), commands.end());
}

/**
 * resolve_absolute_path
 * Resolves the absolute path for target directories for which only a relative
 * path was used.
 *
 * @param directories All target directories of the cd command
 */
void resolve_absolute_path(std::vector<std::string>& directories) {
    for (std::string& entry: directories)
    {
        if (entry.front() == '~' || entry.front() == '/') continue;

        // Get absolute path of a directory
    }
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
