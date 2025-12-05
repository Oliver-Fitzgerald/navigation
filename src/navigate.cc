#include "nlohmann/json.hpp"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cstdlib>
#include <unistd.h>  // for chdir()

struct directory {
    std::string abreviation;
    std::string name;
    std::string path;
};

int readConfig(const char* path, directory (&directories)[10]);
void highligthUnique(directory (&directories)[10], int count);

/*
 * navigate
 * The main event loop for the navigate application
 */
int main(int argc, char** argv) {

    const char* configPath = "/home/guts/projects/personal/navigate/resources/config.json";
    directory directories[10];
    int count;

    // Read Config
    try {
        count  = readConfig(configPath, directories);
    } catch (std::length_error& exception) {
        std::cout << exception.what();
    }

    // Higligth Unique Segment of Name
    highligthUnique(directories, count);

    std::cout << "\nNavigate Menu\n\n";
    for (int index = 0; index < 10 && !directories[index].name.empty(); index++) {

        std::cout << "Name: " << directories[index].name << "\n";
        std::cout << "Absoloute Path: " << directories[index].path << "\n\n";
    }
    
    std::string selection;
    std::cout << "Enter: ";
    std::cin >> selection;

    for (int index = 0; index < std::size(directories); index++) {
        if (directories[index].abreviation == selection) {

            std::ofstream out("/home/guts/projects/personal/navigate/src/navigate_cmd.sh");
            out << "cd " << directories[index].path << "\n";
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
    json data = json::parse(file)["quick_acces_directories"];
    int count;

    if (std::size(data) > 10) {
        throw std::length_error("Only the first 10 entries of the quick_acces_directories will be read");
    }

    for (int index = 0; index < std::size(data) && index < 10; index++) {
        directories[index] = directory{"",data[index]["name"].get<std::string>().c_str(), data[index]["path"].get<std::string>()};
        count = index + 1;
    }

    return count;
}

/**
 * highligthUnique
 * Highligths the unique elements of the begining of each directory.name
 * @param directories The array of 10 directories
 */
void highligthUnique(directory (&directories)[10], int count) {

    for (int i = 0; i < count; i++) {
        
        int matches = 0;
        int last = 0;
        int index = 0;

        for (int j = 0; j < count ; j++) {

            if (i == j) {
                continue;
            }

            if (directories[i].name[index] == directories[j].name[index]) {
                matches++;
            }

            if (j + 1 == count) {
                j++;
                if (directories[i].name[index] == directories[j + 1].name[index]) {
                    matches++;
                }
            }

            if (matches == last) {
                break;
            }
            else {
                j = -1;
                index++;
                last = matches;
            }

        }

        // Finally highligth unique charachters
        std::string directoryName = directories[i].name;

        directories[i].name.clear();
        directories[i].name.resize(50);

        std::strcat(&directories[i].name[0], "(\x1b[32m");

        int y = 0;
        for (int x = 6; x < std::size(directoryName) + 11; x++) {


            directories[i].name[x] = directoryName[y];
            
            if (x - 6 == matches) {
                directories[i].abreviation = directoryName.substr(0, y + 1);
                std::strcat(&directories[i].name[x], "\x1b[0m)");
                x += 5;
            }
            y++;
        }
    }

}
