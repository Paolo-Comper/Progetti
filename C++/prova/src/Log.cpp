#include <iostream>
#include "Log.h"

void log (const char* message,LogType type) {
    switch (type) {
        case INFO:
            std::cout << BLU << "[INFO]" << message << RESET << '\n';
            break;
        case WARNING:
            std::cout << YELLOW << "[WARNING]" << message << RESET << '\n';
            break;
        case ERROR:
        std::cout << RED << "[ERROR]" << message << RESET << '\n';
            break;
    }
    std::cout << message << '\n';
}