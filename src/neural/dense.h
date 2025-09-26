#ifndef DENSE_H
#define DENSE_H

#include <random>
#include <memory>
#include "../matrix/matrix.h"
#include "activation.h"

namespace neural {

class DenseLayer {
private:
    size_t input_size;
    size_t output_size;
    matrix::Matrix weights;
    matrix::Matrix biases;
    std::unique_ptr<Activation> activation;
    
    // Last input and output stored for potential backpropagation
    matrix::Matrix last_input;
    matrix::Matrix last_output;
    matrix::Matrix last_z;  // Pre-activation output

public:
    // Constructor with random initialization
    DenseLayer(size_t input_size, size_t output_size, std::unique_ptr<Activation> activation)
        : input_size(input_size),
          output_size(output_size),
          activation(std::move(activation)) {
        
        // Initialize weights with random values (Xavier/Glorot initialization)
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> dist(-1.0, 1.0);
        
        double scale = std::sqrt(6.0 / (input_size + output_size));
        
        std::vector<std::vector<double>> weight_data(input_size, std::vector<double>(output_size));
        for (size_t i = 0; i < input_size; ++i) {
            for (size_t j = 0; j < output_size; ++j) {
                weight_data[i][j] = dist(gen) * scale;
            }
        }
        weights = matrix::Matrix(weight_data);
        
        // Initialize biases to zero
        biases = matrix::Matrix(1, output_size, 0.0);
    }
    
    // Constructor with provided weights and biases
    DenseLayer(size_t input_size, size_t output_size, 
               const matrix::Matrix& weights, 
               const matrix::Matrix& biases,
               std::unique_ptr<Activation> activation)
        : input_size(input_size),
          output_size(output_size),
          weights(weights),
          biases(biases),
          activation(std::move(activation)) {
        
        // Validate matrix dimensions
        if (weights.getRows() != input_size || weights.getCols() != output_size) {
            throw std::invalid_argument("Weights dimensions don't match layer dimensions");
        }
        
        if (biases.getRows() != 1 || biases.getCols() != output_size) {
            throw std::invalid_argument("Biases dimensions don't match layer dimensions");
        }
    }
    
    // Forward pass
    matrix::Matrix forward(const matrix::Matrix& input) {
        // Validate input dimensions
        if (input.getCols() != input_size) {
            throw std::invalid_argument("Input dimensions don't match layer input size");
        }
        
        // Store input for potential backpropagation
        last_input = input;
        
        // Compute Z = X * W + b
        matrix::Matrix z = input.multiply(weights);
        
        // Add biases to each row
        for (size_t i = 0; i < z.getRows(); ++i) {
            for (size_t j = 0; j < z.getCols(); ++j) {
                z.set(i, j, z.get(i, j) + biases.get(0, j));
            }
        }
        
        // Store pre-activation output
        last_z = z;
        
        // Apply activation function
        last_output = activation->apply(z);
        
        return last_output;
    }
    
    // Getters
    const matrix::Matrix& getWeights() const { return weights; }
    const matrix::Matrix& getBiases() const { return biases; }
    size_t getInputSize() const { return input_size; }
    size_t getOutputSize() const { return output_size; }
    const matrix::Matrix& getLastInput() const { return last_input; }
    const matrix::Matrix& getLastOutput() const { return last_output; }
    const matrix::Matrix& getLastZ() const { return last_z; }
};

} // namespace neural

#endif // DENSE_H
