#include "benchmark.h"
#include "log.h"
#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using std::string, std::vector, std::array;

struct Define {
    string keyword, variable;
};

void loadFile(const string& path, vector<string>& fileText) { // load the file excluding empty lines
    fileText.reserve(50);

    // BENCHMARK_FUNCTION();

    std::ifstream file(path);

    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            if (line.length() != 0) fileText.push_back(line);
        }

        file.close();
    } else {

        log(ERROR, "Could not open file");
    }
}

void cleanCode(vector<string>& fileText, vector<unsigned int>& keyword_index) { // remove normal comments and leave the keywords

    size_t pos_char;
    string* line;
    for (unsigned int i = 0; i < fileText.size(); i++) {
        line = &fileText[i];
        // remove comments that start with # and leve comments thats start with #@
        // leave comments that start with #@
        if (line->find("#@") != std::string::npos) {
            keyword_index.push_back(i);
            continue;
        }
        // remove # and everything after it
        pos_char = line->find("#");
        if (pos_char != std::string::npos) fileText[i] = line->substr(0, line->find('#'));
    }
}

void checkLine(vector<string>& fileText, vector<Define>& defines, vector<unsigned int>& keyword_index) {
    Define define;

    for (string line : fileText) {
        define = {"§§§", "§§§"};

        // if it find a define it will append to defines
        if ((define.keyword != "§§§") && (define.variable != "§§§")) {
            defines.push_back(define);
        }
    }
}

void preprocessorMain(const string& path) {
    // it is a list of the lines of the file separated by '\n'
    vector<string> fileText;

    // is is a list of the keyword and thing of the defines on the code
    vector<Define> defines;
    defines.reserve(10);

    vector<unsigned int> keyword_index;

    loadFile(path, fileText);
    cleanCode(fileText, keyword_index);

    checkLine(fileText, defines, keyword_index);

    std::cout << "size code: " << fileText.size() << " lines" << '\n';
    std::cout << "Number of defines: " << defines.size() << '\n';
    std::cout << "keyword indexes: ";
    for (auto& key : keyword_index) std::cout << key << ", ";
    std::cout << '\n';
    std::cout << "File text: \n";
    for (const string& line : fileText) std::cout << "    " << line << '\n';
}