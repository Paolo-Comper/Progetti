#include <spdlog/spdlog.h>
#include "logger.hpp"

bool Setup()
{
    Logger::Init();
    return false;
}

int main() 
{ 
    if (Setup()) spdlog::error("The program failed to start");

    spdlog::info("ciao");
    return 0;
}