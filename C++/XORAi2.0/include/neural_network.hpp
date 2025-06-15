#pragma once
#include <vector>
#include <optional>

enum class ActivationFunctions 
{
    SIGMOID,
    RELU
};

struct StructureLayer 
{
    unsigned int n_neurons;
    ActivationFunctions activaton_function;
};

class NN
{
  public:

    NN::NN(unsigned int n_imput, std::vector<StructureLayer> nn_structure);
    ~NN();
    void NN::InitRandom(std::optional<unsigned int> seed = std::nullopt);
    void BackPropagation();
    std::vector<double> ForwardPass();

  private:

    struct Layer {
        std::vector<std::vector<double>> weights;
        std::vector<double> bias;
        ActivationFunctions activation_funcion;
    };

    unsigned int m_n_input;

    std::vector<StructureLayer> m_nn_structure;
     
    std::vector<Layer> m_neural_network;

};
