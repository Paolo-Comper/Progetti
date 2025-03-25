#pragma once

#define RESET "\033[0m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define BLU "\033[36m"

enum LogType {
    INFO,
    WARNING,
    ERROR
};

void log(const char *message, LogType type);
void logToFile(const char *message, LogType type);
void svuotaLogFile();
