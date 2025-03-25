#include <iostream>
#include <fstream>
#include <string>
#include <GL/glew.h>
#include "log.h"

//! Per aprire il file di log: tail -f log/log.txt

const char* GLErrorToString(GLenum &error) {
    switch (error) {
        case GL_NO_ERROR: return "GL_NO_ERROR: No error has been recorded.";
        case GL_INVALID_ENUM: return "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.";
        case GL_INVALID_VALUE: return "GL_INVALID_VALUE: A numeric argument is out of range.";
        case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION: The specified operation is not allowed in the current state.";
        case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.";
        case GL_OUT_OF_MEMORY: return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.";
        default: return "Unknown OpenGL error.";

    }
}

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
    case ERROR_GL:
        msg_log = std::string(RED) + "[ERROR]" + "OpenGL:" + message + RESET + '\n';
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
