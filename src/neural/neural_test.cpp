#include "dense.h"
#include "activation.h"
#include <iostream>
#include <vector>
#include <memory>
#include <iomanip>

// Helper function to print a detailed description of a matrix
void printMatrixDetails(const std::string& name, const matrix::Matrix& matrix) {
    std::cout << name << " (" << matrix.getRows() << "x" << matrix.getCols() << "):\n";
    matrix.print();
    std::cout << std::endl;
}

int main() {
    std::cout << "Neural Network Dense Layer Test\n";
    std::cout << "==============================\n\n";

    // Create a simple input dataset (2 features, 3 samples)
    // Each row is a sample, each column is a feature
    std::vector<std::vector<double>> input_data = {
        {0.1, 0.5},  // Sample 1
        {0.2, 0.3},  // Sample 2
        {0.7, 0.9}   // Sample 3
    };
    matrix::Matrix input(input_data);
    printMatrixDetails("Input", input);

    // Create a dense layer with 2 inputs and 3 outputs using ReLU activation
    std::unique_ptr<neural::Activation> relu = std::make_unique<neural::ReLU>();
    neural::DenseLayer layer1(2, 3, std::move(relu));

    // Create example weights manually for demonstration (normally these would be learned)
    std::vector<std::vector<double>> weight_data = {
        {0.1, 0.2, 0.3},  // Weights for input feature 1
        {0.4, 0.5, 0.6}   // Weights for input feature 2
    };
    matrix::Matrix weights(weight_data);
    
    // Create example biases
    std::vector<std::vector<double>> bias_data = {
        {0.1, 0.1, 0.1}  // One bias per output neuron
    };
    matrix::Matrix biases(bias_data);

    // Create another layer with specified weights and ReLU activation
    std::unique_ptr<neural::Activation> relu2 = std::make_unique<neural::ReLU>();
    neural::DenseLayer layer2(2, 3, weights, biases, std::move(relu2));

    // Forward pass through the layer with random weights
    std::cout << "Testing layer with random initialization:\n";
    printMatrixDetails("Weights", layer1.getWeights());
    printMatrixDetails("Biases", layer1.getBiases());
    
    matrix::Matrix output1 = layer1.forward(input);
    printMatrixDetails("Output from randomly initialized layer", output1);

    // Forward pass through the layer with specified weights
    std::cout << "\nTesting layer with specified weights:\n";
    printMatrixDetails("Weights", layer2.getWeights());
    printMatrixDetails("Biases", layer2.getBiases());
    
    matrix::Matrix output2 = layer2.forward(input);
    printMatrixDetails("Output from layer with specified weights", output2);

    // Expected output calculation for verification:
    // Sample 1: [0.1, 0.5] * [0.1, 0.2, 0.3; 0.4, 0.5, 0.6] + [0.1, 0.1, 0.1]
    //         = [0.1*0.1 + 0.5*0.4, 0.1*0.2 + 0.5*0.5, 0.1*0.3 + 0.5*0.6] + [0.1, 0.1, 0.1]
    //         = [0.01 + 0.20, 0.02 + 0.25, 0.03 + 0.30] + [0.1, 0.1, 0.1]
    //         = [0.21, 0.27, 0.33] (before ReLU)
    //         = [0.21, 0.27, 0.33] (after ReLU, all positive)
    std::cout << "\nExpected output for first sample:\n";
    std::cout << "0.21, 0.27, 0.33\n\n";
    
    // Now test with sigmoid activation
    std::cout << "Testing with Sigmoid activation:\n";
    std::unique_ptr<neural::Activation> sigmoid = std::make_unique<neural::Sigmoid>();
    neural::DenseLayer sigmoid_layer(2, 3, weights, biases, std::move(sigmoid));
    
    matrix::Matrix sigmoid_output = sigmoid_layer.forward(input);
    printMatrixDetails("Sigmoid output", sigmoid_output);
    
    // Test with tanh activation
    std::cout << "Testing with Tanh activation:\n";
    std::unique_ptr<neural::Activation> tanh = std::make_unique<neural::Tanh>();
    neural::DenseLayer tanh_layer(2, 3, weights, biases, std::move(tanh));
    
    matrix::Matrix tanh_output = tanh_layer.forward(input);
    printMatrixDetails("Tanh output", tanh_output);
    
    return 0;
}
