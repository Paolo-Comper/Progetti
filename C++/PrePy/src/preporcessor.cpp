#include "log.h"
#include <fstream>
#include <string>
#include <vector>
#include <iostream>

using std::string, std::vector;

void loadFile(const string &path, vector<string>& fileText) {

    std::ifstream file(path);

    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            fileText.push_back(line);
        }

        file.close();
    } else {

        log(ERROR, "Could not open file");
    }
}

void preprocessorMain(const string &path) {

    vector<string> fileText; // it is a list of the lines of the file separated by '\n'
    fileText.reserve(50);

    loadFile(path, fileText);
    
    std::cout << "size: " << fileText.size() << " lines" << '\n';

    std::cout << "File text: \n";
    for (const string& line : fileText) {
        std::cout << line << '\n';
    }
}