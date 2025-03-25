#include <iostream>
#include <fstream>
#include <string>
#include "log.h"

//! Per aprire il file di log: tail -f log/log.txt

std::string logMsg(const char *message, LogType type) {
    std::string msg_log;
    switch (type) {
    case INFO:
        msg_log = std::string(BLU) + "[INFO]" + message + RESET + '\n';
        break;
    case WARNING:
        msg_log = std::string(YELLOW) + "[WARNING]" + message + RESET + '\n';
        break;
    case ERROR:
        msg_log = std::string(RED) + "[ERROR]" + message + RESET + '\n';
        break;
    }
    return msg_log;
}

void log(const char* message, LogType type) {
    std::cout << logMsg(message, type);
}

void logToFile(const char *message, LogType type) {
    std::ofstream logFile("log/log.txt", std::ios::app);
    if (logFile.is_open()) {
        logFile << logMsg(message, type);
        logFile.close();
    }
}

void svuotaLogFile()
{
    std::ofstream file("log/log.txt", std::ios::out | std::ios::trunc); // Modalità out e trunc per svuotare il file
    if (file.is_open()) {
        // Scriviamo un messaggio vuoto per svuotare il file senza interrompere tail
        file.close(); // Chiudiamo il file dopo averlo svuotato
        log("Il file di log è stato svuotato.", INFO);
    }
    else log("Errore nell'aprire il file di log.", ERROR);
}
