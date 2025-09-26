#ifndef ACTIVATION_H
#define ACTIVATION_H

#include <cmath>
#include "../matrix/matrix.h"

namespace neural {

class Activation {
public:
    virtual ~Activation() = default;
    
    // Apply activation function to a matrix
    virtual matrix::Matrix apply(const matrix::Matrix& input) const = 0;
    
    // Compute derivative for backpropagation (if needed later)
    virtual matrix::Matrix derivative(const matrix::Matrix& input) const = 0;
};

class ReLU : public Activation {
public:
    matrix::Matrix apply(const matrix::Matrix& input) const override {
        matrix::Matrix result(input.getRows(), input.getCols());
        
        for (size_t i = 0; i < input.getRows(); ++i) {
            for (size_t j = 0; j < input.getCols(); ++j) {
                double value = input.get(i, j);
                result.set(i, j, value > 0 ? value : 0.0);
            }
        }
        
        return result;
    }
    
    matrix::Matrix derivative(const matrix::Matrix& input) const override {
        matrix::Matrix result(input.getRows(), input.getCols());
        
        for (size_t i = 0; i < input.getRows(); ++i) {
            for (size_t j = 0; j < input.getCols(); ++j) {
                result.set(i, j, input.get(i, j) > 0 ? 1.0 : 0.0);
            }
        }
        
        return result;
    }
};

class Sigmoid : public Activation {
public:
    matrix::Matrix apply(const matrix::Matrix& input) const override {
        matrix::Matrix result(input.getRows(), input.getCols());
        
        for (size_t i = 0; i < input.getRows(); ++i) {
            for (size_t j = 0; j < input.getCols(); ++j) {
                double x = input.get(i, j);
                result.set(i, j, 1.0 / (1.0 + std::exp(-x)));
            }
        }
        
        return result;
    }
    
    matrix::Matrix derivative(const matrix::Matrix& input) const override {
        matrix::Matrix sigmoid_output = apply(input);
        matrix::Matrix result(input.getRows(), input.getCols());
        
        for (size_t i = 0; i < input.getRows(); ++i) {
            for (size_t j = 0; j < input.getCols(); ++j) {
                double s = sigmoid_output.get(i, j);
                result.set(i, j, s * (1.0 - s));
            }
        }
        
        return result;
    }
};

class Tanh : public Activation {
public:
    matrix::Matrix apply(const matrix::Matrix& input) const override {
        matrix::Matrix result(input.getRows(), input.getCols());
        
        for (size_t i = 0; i < input.getRows(); ++i) {
            for (size_t j = 0; j < input.getCols(); ++j) {
                double x = input.get(i, j);
                result.set(i, j, std::tanh(x));
            }
        }
        
        return result;
    }
    
    matrix::Matrix derivative(const matrix::Matrix& input) const override {
        matrix::Matrix tanh_output = apply(input);
        matrix::Matrix result(input.getRows(), input.getCols());
        
        for (size_t i = 0; i < input.getRows(); ++i) {
            for (size_t j = 0; j < input.getCols(); ++j) {
                double t = tanh_output.get(i, j);
                result.set(i, j, 1.0 - t * t);
            }
        }
        
        return result;
    }
};

} // namespace neural

#endif // ACTIVATION_H
