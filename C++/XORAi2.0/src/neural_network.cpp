#include "neural_network.hpp"
#include "logger.hpp"
#include <random>

NN::NN(uint32_t n_imput, std::vector<StructureLayer> nn_structure)
    : m_nn_structure(nn_structure), m_n_input(n_imput) 
{

}

NN::~NN() 
{

}

void NN::InitRandom(std::optional<unsigned int> seed) { 

	if (!seed) 
    {
        // Genera un seed casuale se non inserito
        // con le parentesi {} lo uso come oggetto 
        // temporaneo solo per quel seed
        seed = std::random_device{}();
    }

    spdlog::info("Seed: {}", *seed);

    // Usa il seed per inizializzare il generatore
    std::mt19937 generator(*seed); 

    // Crea una distribuzione di numeri da -1 a 1
    std::uniform_real_distribution<double> distribuzione(-1, 1);

    for (uint32_t n_layer = 0; n_layer < m_nn_structure.size(); n_layer++)
    {
        Layer layer_corrente;
        
        // Inizializa i neuroni
        for (uint32_t n_neuron = 0; n_neuron < m_nn_structure[n_layer].n_neurons; n_neuron++)
        {
            // gestisce il fatto che il numero di neutoni 
            // precedente al primo layer è il numero di imput
            uint32_t n_prev_neurons =
                (n_layer == 0) ? m_n_input : m_nn_structure[n_layer - 1].n_neurons;

            // Inizializza i weights per il neurone corrente
                std::vector<double> t_weights;

                for (uint32_t i = 0; i < n_prev_neurons; i++) 
                    t_weights.emplace_back(distribuzione(generator));
            
                layer_corrente.weights.emplace_back(t_weights);

            // Inizializza i bias per il neurone corrente
                layer_corrente.bias.emplace_back(distribuzione(generator));
        }

        m_neural_network.emplace_back(layer_corrente);
    }
}

void NN::BackPropagation() 
{ 

}

std::vector<double> NN::ForwardPass() 
{ 
	return std::vector<double>(); 
}
