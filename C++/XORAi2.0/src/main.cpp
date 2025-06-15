#include <spdlog/spdlog.h>
#include "logger.hpp"
#include "neural_network.hpp"
#include <vector>

bool Setup()
{
    Logger::Init();
    return false;
}

int main() 
{ 
    if (Setup()) spdlog::error("The program failed to start");

    NN MyNN(2,
            {{3, ActivationFunctions::SIGMOID},
             {3, ActivationFunctions::SIGMOID},
             {2, ActivationFunctions::RELU}});

    MyNN.InitRandom();

    return 0;
}