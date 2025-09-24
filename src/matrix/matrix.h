#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <stdexcept>
#include <iostream>

namespace matrix {

class Matrix {
private:
    std::vector<std::vector<double>> data;
    size_t rows;
    size_t cols;

public:
    // Default constructor
    Matrix();
    
    // Constructor with dimensions
    Matrix(size_t rows, size_t cols);
    
    // Constructor with dimensions and initial value
    Matrix(size_t rows, size_t cols, double initial_value);
    
    // Constructor with data
    Matrix(const std::vector<std::vector<double>>& data);
    
    // Copy constructor
    Matrix(const Matrix& other);
    
    // Get number of rows
    size_t getRows() const;
    
    // Get number of columns
    size_t getCols() const;
    
    // Access element (for reading)
    double get(size_t row, size_t col) const;
    
    // Set element value
    void set(size_t row, size_t col, double value);
    
    // Matrix multiplication
    Matrix multiply(const Matrix& other) const;
    
    // Static matrix multiplication function
    static Matrix multiply(const Matrix& a, const Matrix& b);
    
    // Print matrix
    void print() const;
};

} // namespace matrix

#endif // MATRIX_H
