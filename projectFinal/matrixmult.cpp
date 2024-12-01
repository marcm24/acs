#include <iostream>
#include <vector>
#include <chrono> // for high res clock

// function to perform matrix multiplication
void matrixMultiply(const std::vector<std::vector<int>>& A, 
                    const std::vector<std::vector<int>>& B, 
                    std::vector<std::vector<int>>& C, 
                    int N) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < N; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() {
    const int N = 1000; // matrix dimension
    std::vector<std::vector<int>> A(N, std::vector<int>(N));
    std::vector<std::vector<int>> B(N, std::vector<int>(N));
    std::vector<std::vector<int>> C(N, std::vector<int>(N, 0));

    // initialize matrices A and B with random values
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            A[i][j] = rand() % 10;
            B[i][j] = rand() % 10;
        }
    }

    // measure the execution time
    auto start = std::chrono::high_resolution_clock::now();

    // perform matrix multiplication
    matrixMultiply(A, B, C, N);

    auto stop = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = stop - start; // timing result

    // output the result matrix
    std::cout << "Result Matrix (C):\n";
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            std::cout << C[i][j] << " ";
        }
        std::cout << "\n";
    }

    // output the time taken
    std::cout << "\nTime taken for matrix multiplication: " << duration.count() << " ms\n";

    return 0;
}
