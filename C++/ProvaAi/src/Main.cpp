#include <vector>
#include <random>
#include <iostream>
#include <cmath>
#include "Data.h"

using std::vector, std::cout;

class NeuralNetwork
{
private:
    vector<vector<double>> layers;
    vector<vector<vector<double>>> weights;
    vector<vector<double>> biases;
    double learning_rate;

public:
    NeuralNetwork(const vector<int> &struttura, double lr = 0.1) : learning_rate(lr)
    {
        for (int neurons : struttura)
            layers.push_back(vector<double>(neurons, 0.0));

        for (size_t i = 0; i < struttura.size() - 1; ++i)
        {
            weights.push_back(randomWeights(struttura[i], struttura[i + 1]));
            biases.push_back(vector<double>(struttura[i + 1], 0.0));
        }
    }

    vector<vector<double>> randomWeights(int prev_n_neuroni, int next_n_neuroni)
    {
        vector<vector<double>> matrix(prev_n_neuroni, vector<double>(next_n_neuroni));
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(-1.0, 1.0);

        for (int i = 0; i < prev_n_neuroni; ++i)
            for (int j = 0; j < next_n_neuroni; ++j)
                matrix[i][j] = dis(gen);

        return matrix;
    }

    double sigmoid(double x)
    {
        return 1.0 / (1.0 + exp(-x));
    }

    double sigmoidDerivative(double x)
    {
        double sig = sigmoid(x);
        return sig * (1 - sig);
    }

    void forwardPass(const vector<double> &input)
    {
        layers[0] = input;
        for (size_t i = 0; i < weights.size(); ++i)
        {
            for (size_t j = 0; j < layers[i + 1].size(); ++j)
            {
                layers[i + 1][j] = biases[i][j];
                for (size_t k = 0; k < layers[i].size(); ++k)
                    layers[i + 1][j] += layers[i][k] * weights[i][k][j];
                layers[i + 1][j] = sigmoid(layers[i + 1][j]);
            }
        }
    }

    double getCost(const vector<double> &out_teorico)
    {
        vector<double> out_pratico = layers.back();
        double cost = 0;
        for (size_t i = 0; i < out_pratico.size(); ++i)
            cost += pow(out_pratico[i] - out_teorico[i], 2);
        return cost / out_pratico.size();
    }

    void backpropagation(const vector<double> &out_teorico)
    {
        vector<vector<double>> deltas(layers.size());
        deltas.back() = vector<double>(layers.back().size());

        for (size_t i = 0; i < layers.back().size(); ++i)
            deltas.back()[i] = (out_teorico[i] - layers.back()[i]) * sigmoidDerivative(layers.back()[i]);

        for (int l = layers.size() - 2; l > 0; --l)
        {
            deltas[l] = vector<double>(layers[l].size(), 0.0);
            for (size_t j = 0; j < layers[l].size(); ++j)
                for (size_t k = 0; k < layers[l + 1].size(); ++k)
                    deltas[l][j] += deltas[l + 1][k] * weights[l][j][k] * sigmoidDerivative(layers[l][j]);
        }

        for (size_t l = 0; l < weights.size(); ++l)
        {
            for (size_t j = 0; j < layers[l + 1].size(); ++j)
            {
                biases[l][j] += learning_rate * deltas[l + 1][j];
                for (size_t k = 0; k < layers[l].size(); ++k)
                    weights[l][k][j] += learning_rate * layers[l][k] * deltas[l + 1][j];
            }
        }
    }

    vector<double> getOutput()
    {
        return layers.back();
    }
};

int main()
{
    vector<int> struttura = {4, 5, 6, 7};
    NeuralNetwork nn(struttura, 0.01);

    vector<double> input = {0.0, 0.0, 0.0, 1.0};
    //                         AND  NAND OR   NOR  XOR  XNOR NONE
    vector<double> expected = {1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0};

    for (int epoch = 0; epoch < 10'000'000; ++epoch)
    {
        uint8_t random = std::rand();
        random = random >> 4;
        
        nn.forwardPass(input_data[random]);
        nn.backpropagation(output_data[random]);
        if (epoch % 1000 == 0)
        {
            cout << "Epoch " << epoch << " Cost: " << nn.getCost(output_data[random]) << '\n';
        }
    }

    nn.forwardPass(input);
    auto output = nn.getOutput();
    
    cout << "AND: "  << output[0] - expected[0] << '\n';
    cout << "NAND: " << output[1] - expected[1] << '\n';
    cout << "OR: "   << output[2] - expected[2] << '\n';
    cout << "NOR: "  << output[3] - expected[3] << '\n';
    cout << "XOR: "  << output[4] - expected[4] << '\n';
    cout << "XNOR: " << output[5] - expected[5] << '\n';
    cout << "NONE: " << output[6] - expected[6] << '\n';
    cout << "---------------------------------------\n";
    cout << "AND: "  << output[0] << '\n';
    cout << "NAND: " << output[1] << '\n';
    cout << "OR: "   << output[2] << '\n';
    cout << "NOR: "  << output[3] << '\n';
    cout << "XOR: "  << output[4] << '\n';
    cout << "XNOR: " << output[5] << '\n';
    cout << "NONE: " << output[6] << '\n';
    

    return 0;
}


