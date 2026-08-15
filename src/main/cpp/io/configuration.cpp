/*
 * configuration.cpp
 * Provides functions for reading and writing to and from configuration files
 *
 * Configuration Files:
 *  - src/main/resources/directories.json : User Configured Paths
 */

// standard
#include <fstream>
// json
#include <nlohmann/json.hpp>

/**
 * read_config
 * Reads the user config
 *
 * @param path          The path to the user configuration file (json)
 * @param directories   The directories metadata read in from the configuration
 * @return              The count of directory object in the config
 */
int read_config(const char* path, directory (&directories)[10])
{
    using json = nlohmann::json;

    std::ifstream file(path);
    json data = json::parse(file)["quick_access_directories"];

    if (std::size(data) > 10) {
        std::cout << "[INFO] quick_access_directories exceeds 10 entries, only the first 10 will be read";
    }

    int count = 0;
    while (count < std::size(data) && count < 10) {
        directories[count] = directory{data[count]["name"].get<std::string>().c_str(),
                                       data[count]["path"].get<std::string>() };
        count++;
    }

    return count;
}

/**
 * append_most_frequent_paths
 * Appends those paths that are most frequently visited by the user (weekly)
 * to the list of ... TODO
 */
void append_most_frequent_paths(int countPreConfigured, directory (&directories)[10])
{
}
