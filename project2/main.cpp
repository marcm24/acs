#include <iostream>
#include <cstdlib>
#include <ctime>  
#include <vector>

std::vector<std::vector<int>> generateMatrix(int rows, int cols, std::string fill) {
    std::vector<std::vector<int>> matrix(rows, std::vector<int>(cols, 0));
    double threshold;

    // threshold based on fill type
    if (fill == "dense") {
        threshold = 0.9; 
    } else {
        threshold = 0.1; // for sparse matrices
    }

    // random number generation
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            // Random pick of 0 or a number between 1 and 10
            double randNum = static_cast<double>(std::rand()) / RAND_MAX;

            // Fill matrix based on threshold
            if (randNum < threshold) {
                // Generate a random number between 1 and 10 for non-zero values
                matrix[i][j] = std::rand() % 10 + 1; // Generates a number from 1 to 10
            }
        }
    }

    return matrix;
}

// Function to multiply two matrices
std::vector<std::vector<int>> multiplyMatrices(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B) {
    int rowsA = A.size();
    int colsA = A[0].size();
    int rowsB = B.size();
    int colsB = B[0].size();

    // check if multiplication is possible
    if (colsA != rowsB) {
        std::cerr << ("error: matrix dimensions are incompatible for multiplication") << std::endl;
    }

    // resulting matrix will have dimensions rowsA x colsB
    std::vector<std::vector<int>> result(rowsA, std::vector<int>(colsB, 0));

    // multiplication
    for (int i = 0; i < rowsA; ++i) {
        for (int j = 0; j < colsB; ++j) {
            for (int k = 0; k < colsA; ++k) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return result;
}

// Function to display a matrix
void displayMatrix(const std::vector<std::vector<int>>& matrix) {
    for (const auto& row : matrix) {
        for (int elem : row) {
            std::cout << elem << " ";
        }
        std::cout << std::endl;
    }
}

int main(int argc, char *argv[]) {
    // multiple threads, x86 SIMD instructions, minimize cache miss
    try {
        if (argc < 2) {
            throw std::runtime_error("error: expected specification for threading");
        }
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    } 

    std::string threading = argv[1];

    if (threading == "mt") { // mt for multiple threads

    } else if (threading == "simd") { // simd for x86 instructions

    } else if (threading == "cache") { // cache to minimize cache misses

    }

    int rowsA = 3;
    int colsA = 2;
    int rowsB = 2;
    int colsB = 4;

    // matrices
    std::vector<std::vector<int>> A = generateMatrix(rowsA, colsA, "dense");
    std::vector<std::vector<int>> B = generateMatrix(rowsB, colsB, "dense");

    std::cout << "matrix A:" << std::endl;
    displayMatrix(A);

    std::cout << "matrix B:" << std::endl;
    displayMatrix(B);

    // multiply matrices A and B
    try {
        std::vector<std::vector<int>> C = multiplyMatrices(A, B);
        std::cout << "matrix C = (A * B):" << std::endl;
        displayMatrix(C);
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}