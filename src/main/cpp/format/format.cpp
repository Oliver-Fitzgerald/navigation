/* 
 * format.cpp
 * Provides utilities for formating output data
 */

namespace format
{

/**
 * highlight_unique
 * highlights the unique elements of the begining of each directory.name
 * @param directories The array of 10 directories
 */
void highlight_unique(directory (&directories)[10], int count)
{

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

}
