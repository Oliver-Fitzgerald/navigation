#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cstdlib>
#include <string>
#include <unistd.h>  // for chdir()

struct directory {
    std::string abbreviation;
    std::string name;
    std::string path;
};

int readConfig(const char* path, directory (&directories)[10]);
void highlightUnique(directory (&directories)[10], int count);

/*
 * navigate
 * The main event loop for the navigate application
 */
int main(int argc, char** argv) {

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

/**
 * readConfig
 * Reads in the config for this command
 * @return The count of directory object in the config
 */
int readConfig(const char* path, directory (&directories)[10]) {
    using json = nlohmann::json;

    std::ifstream file(path);
    json data = json::parse(file)["quick_access_directories"];
    int count;

    if (std::size(data) > 10) {
        std::cout << "[INFO] quick_access_directories exceeds 10 entries, only the first 10 will be read";
    }

    for (int index = 0; index < std::size(data) && index < 10; index++) {
        directories[index] = directory{"",data[index]["name"].get<std::string>().c_str(), data[index]["path"].get<std::string>()};
        count = index + 1;
    }

    return count;
}

/**
 * highlightUnique
 * highlights the unique elements of the begining of each directory.name
 * @param directories The array of 10 directories
 */
void highlightUnique(directory (&directories)[10], int count) {

    std::string one = "(\x1b[32m";
    std::string two = "\x1b[0m)";

    // initialization
    for (int index = 0; index < count; index++) {
        directories[index].name.insert(directories[index].name.begin(),one.begin(),one.end());
    }

    // construct binary tree of unique characters  

    // highlight unique characters
    for (int index = 0; index < count; index++) {
        directories[index].name.insert(directories[index].name.end(),two.begin(),two.end());
    }
}
