#pragma once
#include <vector>

class NN
{
public:
    NN();
    ~NN();
    void InitWeights();
    void BackPropagation();
    std::vector<double> ForwardPass();

private:
    std::vector<std::vector<std::vector<double>>> weights;
    std::vector<std::vector<double>> biases;

};
