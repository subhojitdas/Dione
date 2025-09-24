#include "matrix.h"
#include <iostream>
#include <vector>

int main() {
    std::cout << "Matrix Multiplication Test\n";
    std::cout << "=========================\n";

    // Create matrix A
    std::vector<std::vector<double>> dataA = {
        {1.0, 2.0, 3.0},
        {4.0, 5.0, 6.0}
    };
    matrix::Matrix A(dataA);
    std::cout << "Matrix A (" << A.getRows() << "x" << A.getCols() << "):\n";
    A.print();
    std::cout << "\n";

    // Create matrix B
    std::vector<std::vector<double>> dataB = {
        {7.0, 8.0},
        {9.0, 10.0},
        {11.0, 12.0}
    };
    matrix::Matrix B(dataB);
    std::cout << "Matrix B (" << B.getRows() << "x" << B.getCols() << "):\n";
    B.print();
    std::cout << "\n";

    // Multiply matrices
    try {
        std::cout << "Multiplying A * B:\n";
        matrix::Matrix C = A.multiply(B);
        std::cout << "Result (" << C.getRows() << "x" << C.getCols() << "):\n";
        C.print();
        std::cout << "\n";

        // Verify result: A * B should be:
        // [1*7 + 2*9 + 3*11, 1*8 + 2*10 + 3*12]
        // [4*7 + 5*9 + 6*11, 4*8 + 5*10 + 6*12]
        // = [58, 64]
        //   [139, 154]
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    // Try invalid multiplication (dimensions mismatch)
    try {
        std::cout << "Attempting invalid multiplication B * B:\n";
        matrix::Matrix invalid = B.multiply(B);
        invalid.print();  // This should not execute
    } catch (const std::exception& e) {
        std::cout << "Caught expected error: " << e.what() << "\n\n";
    }

    // Test other constructors
    std::cout << "Creating a 2x3 matrix initialized with zeros:\n";
    matrix::Matrix zeros(2, 3);
    zeros.print();
    std::cout << "\n";

    std::cout << "Creating a 2x2 matrix initialized with value 42:\n";
    matrix::Matrix filled(2, 2, 42.0);
    filled.print();
    std::cout << "\n";

    return 0;
}
