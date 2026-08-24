/*
 * main
 * The entrypoint of the program
 */

// standard
#include <iostream>
// navigate
#include "util/binary_tree.cpp"
#include "data_objects/directory.cpp"
#include "io/configuration.cpp"

std::string USER = std::getenv("USER");
std::string CONFIG_PATH = "/home/" + USER + "/projects/personal/navigation/src/main/resources/directories.json";

int main(int argc, char** argv)
{
    // read config
    int countPreConfigured; // number of directories read from user configuration
    directory directories[10];
    try {
        countPreConfigured = read_config(CONFIG_PATH.c_str(), directories);

    } catch (std::length_error& exception) {
        std::cerr << exception.what();
    }

    // append most frequently visited directories
    try {
        append_most_frequent_paths(countPreConfigured, directories);
    } catch(std::exception& exception) {
        std::cerr << exception.what();
    }

    // format prompt
    // update command

    return 0;
}
