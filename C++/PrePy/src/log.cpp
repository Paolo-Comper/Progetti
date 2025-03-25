#include <iostream>
#include <fstream>
#include <string>
#include "log.h"

using std::string;

//! Per aprire il file di log: tail -f log/log.txt

string logMsg(LogType type, const char *message) {
    string msg_log;
    switch (type) {
    case INFO:
        return string(BLUE) + "[INFO] " + message + RESET + '\n';
    case WARNING:
        return string(YELLOW) + "[WARNING] " + message + RESET + '\n';
    case ERROR:
        return string(RED) + "[ERROR] " + message + RESET + '\n';
    }
    return "error in logFuncion";
}

void log(LogType type, const char* message) {
    std::cout << logMsg(type, message);
}

void logToFile(LogType type, const char *message) {
    std::ofstream logFile("log/log.txt", std::ios::app);
    if (logFile.is_open()) {
        logFile << logMsg(type, message);
        logFile.close();
    }
}

void svuotaLogFile() {
    std::ofstream file("log/log.txt", std::ios::out | std::ios::trunc); // Modalità out e trunc per svuotare il file
    if (file.is_open()) {
        // Scriviamo un messaggio vuoto per svuotare il file senza interrompere tail
        file.close(); // Chiudiamo il file dopo averlo svuotato
        log(INFO, "Il file di log è stato svuotato.");
    }
    else log(ERROR, "Errore nell'aprire il file di log.");
}
