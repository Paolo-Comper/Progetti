#pragma once

#define RESET "\033[0m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define BLUE "\033[36m"

enum LogType {
    INFO,
    WARNING,
    ERROR
};

void log(LogType type, const char *message);
void logToFile(LogType type, const char *message);
void svuotaLogFile();
