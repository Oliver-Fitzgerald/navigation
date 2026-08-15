#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cstdlib>
#include <string>
#include <unistd.h>  // for chdir()

struct directory {
    std::string name;
    std::string path;
};
/*
 * navigate
 * The main event loop for the navigate application
 */
int navigate(int argc, char** argv) {

    const char* configPath = "/home/ollie/projects/personal/navigation/src/main/resources/directories.json";
    directory directories[10];
    int count;

    // Read Config
    try {
        count  = readConfig(configPath, directories);
    } catch (std::length_error& exception) {
        std::cerr << exception.what();
    }

    try {
        // Highlight Unique Segment of Name
        highlightUnique(directories, count);
    } catch (std::exception& exception) {
        std::cout << "[WARNING] Failed to highlight unique elements of target names";
        // std::cerr << exception.what();
    }

    std::cout << "\nNavigate Menu\n\n";
    for (int index = 0; index < 10 && !directories[index].name.empty(); index++) {

        std::cout << "Name: " << directories[index].name << "\n";
        std::cout << "Absolute Path: " << directories[index].path << "\n\n";
    }
    
    std::string selection;
    std::cout << "Enter: ";
    std::cin >> selection;

    for (int index = 0; index < std::size(directories); index++) {
        if (directories[index].abbreviation == selection) {

            std::ofstream out("/home/ollie/projects/personal/navigation/src/main/resources/navigate_cmd.sh");
            out << "cd " << directories[index].path << "\n";
            std::cout << "Navigating to path: " << directories[index].path << "\n";
            out.close();
            std::cout << "Run: source navigate_cmd.sh\n";
        }
    }
    return 0;
}

