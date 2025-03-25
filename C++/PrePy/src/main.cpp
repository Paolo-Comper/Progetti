#include <iostream>
#include <cctype>
#include "log.h"
#include <cstdlib>
#include <string>
#include "config_menu.h"

using std::string;

//! Check if the arguments are valid for the program
bool checkValidArguments(const int &n_arg, const string &input_path) {

    //? Check if the number of arguments is correct
    if (n_arg == 2) {
        log(INFO, "Output path not declared, using default path");
    }
    else if (n_arg > 3) {
        log(ERROR, "To many arguments: Expected input_path output_path(optional)");
        return false;
    }

    //? Check if the input file exists
    if (FILE *file = fopen(input_path.c_str(), "r")) {
        fclose(file);
    } 
    else {
        log(ERROR, "Input file does not exist");
        return false;
    }

    //? Check if is a python file
    if (input_path.substr(input_path.find_last_of(".") + 1) != "py") {
        log(ERROR, "Input file is not a python file");
        return false;
    }
    return true;
}

//! Check if the user wants to open the config menu
bool checkConfigMenu(const string &input) {
    return (input == "config");
}



int main(int n_arg, char* args[]) {
    if (n_arg < 2) {
        log(ERROR, "Missing arguments: Expected input_path or 'config'");
        return 1;
    }
    else if (checkConfigMenu(args[1])) {
        mainMenu();
        return 0;
    }
    else if (checkValidArguments(n_arg, args[1])) {
        //TODO: Implement the main function
        log(INFO, "Arguments are valid");
        return 0;
    }
    else {
        return 0;
        
    }



}
