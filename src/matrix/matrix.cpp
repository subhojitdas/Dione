#include "matrix.h"

namespace matrix {

// Default constructor
Matrix::Matrix() : rows(0), cols(0) {}

// Constructor with dimensions
Matrix::Matrix(size_t rows, size_t cols) : rows(rows), cols(cols) {
    data.resize(rows, std::vector<double>(cols, 0.0));
}

// Constructor with dimensions and initial value
Matrix::Matrix(size_t rows, size_t cols, double initial_value) : rows(rows), cols(cols) {
    data.resize(rows, std::vector<double>(cols, initial_value));
}

// Constructor with data
Matrix::Matrix(const std::vector<std::vector<double>>& data) {
    if (data.empty()) {
        rows = 0;
        cols = 0;
    } else {
        rows = data.size();
        cols = data[0].size();
        
        // Check if all rows have the same number of columns
        for (const auto& row : data) {
            if (row.size() != cols) {
                throw std::invalid_argument("All rows must have the same number of columns");
            }
        }
        
        this->data = data;
    }
}

// Copy constructor
Matrix::Matrix(const Matrix& other) : data(other.data), rows(other.rows), cols(other.cols) {}

// Get number of rows
size_t Matrix::getRows() const {
    return rows;
}

// Get number of columns
size_t Matrix::getCols() const {
    return cols;
}

// Access element (for reading)
double Matrix::get(size_t row, size_t col) const {
    if (row >= rows || col >= cols) {
        throw std::out_of_range("Matrix indices out of range");
    }
    return data[row][col];
}

// Set element value
void Matrix::set(size_t row, size_t col, double value) {
    if (row >= rows || col >= cols) {
        throw std::out_of_range("Matrix indices out of range");
    }
    data[row][col] = value;
}

// Matrix multiplication
Matrix Matrix::multiply(const Matrix& other) const {
    return Matrix::multiply(*this, other);
}

// Static matrix multiplication function
Matrix Matrix::multiply(const Matrix& a, const Matrix& b) {
    // Check if matrices can be multiplied
    if (a.getCols() != b.getRows()) {
        throw std::invalid_argument("Matrix dimensions mismatch for multiplication: "
                                   + std::to_string(a.getRows()) + "x" + std::to_string(a.getCols())
                                   + " and " + std::to_string(b.getRows()) + "x" + std::to_string(b.getCols()));
    }
    
    // Create result matrix initialized with zeros
    Matrix result(a.getRows(), b.getCols());
    
    // Perform matrix multiplication
    for (size_t i = 0; i < a.getRows(); ++i) {
        for (size_t j = 0; j < b.getCols(); ++j) {
            double sum = 0.0;
            for (size_t k = 0; k < a.getCols(); ++k) {
                sum += a.get(i, k) * b.get(k, j);
            }
            result.set(i, j, sum);
        }
    }
    
    return result;
}

// Print matrix
void Matrix::print() const {
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            std::cout << data[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

} // namespace matrix
