#include "log.h"
#include <iostream>
#include <string>
#include "config_menu.h"
#include "gui.h"

#define OS 'L' // Windows(W), Linux(L)

int printMenu(std::string &menu){
    // Clear the console
    #if OS == 'W'
    system("cls");
    #elif OS == 'L'
    system("clear");
    #endif

    std::cout << BLUE << menu << RESET << '\n';
    std::cout << "Choose an option: ";
    int option;
    std::cin >> option;

    return option;
}

void preferenceMenu(){
    int option = -1;

    while (option != 4){
        option = printMenu(gui::preference_menu);

        switch (option) {
            case 1: // Default output path
                break;
    
            case 2: // Temp file auto delete
                break;
    
            case 3: // Encoding
                break;
    
            case 4: //Exit
                return;
        } 
    }
}

void settingsMenu(){
    int option = -1;

    while (option != 4){
        option = printMenu(gui::settings_menu);

        switch (option) {
            case 1: // Enable multy-threading
                break;
    
            case 2: // Max thred
                break;
    
            case 3: // Timeout
                break;
    
            case 4: //Exit
                return;
        } 
    }
}

void securityMenu(){
    int option = -1;

    while (option != 4){
        option = printMenu(gui::security_menu);

        switch (option) {
            case 1: // Ask for confirm
                break;
    
            case 2: // Limit RAM usage
                break;
    
            case 3: // Limit CPU usage
                break;
    
            case 4: //Exit
                return;
        } 
    }
}

void loggingMenu(){
    int option = -1;

    while (option != 3){
        option = printMenu(gui::logging_menu);

        switch (option) {
            case 1: // Logging level
                break;
            
            case 2: // Log to file
                break;

        }
    }
}

void ottimizationsMenu(){
    int option = -1;

    while (option != 3){
        option = printMenu(gui::ottimizzations_menu);

        switch (option){
            case 1: // Remove comment
                break;
            
            case 2: // Clean syntax
                break;

        }
    }
}

void mainMenu(){
    int option = -1;

    while (option != 6){
        option = printMenu(gui::main_menu);

        switch (option) {
            case 1: // Preference
                preferenceMenu();
                break;
    
            case 2: // Settings
                settingsMenu();
                break;
    
            case 3: // Security
                securityMenu();
                break;
    
            case 4: // Logging & debug
                loggingMenu();
                break;
    
            case 5: // Ottimizations
                ottimizationsMenu();
                break;
        } 
    } 
}