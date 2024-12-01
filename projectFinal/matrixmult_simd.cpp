#include <iostream>
#include <vector>
#include <chrono> // for high res clock
#include <immintrin.h> // for AVX/SSE

// function to perform matrix multiplication using SIMD
void matrixMultiplySIMD(const std::vector<std::vector<int>>& A, 
                        const std::vector<std::vector<int>>& B, 
                        std::vector<std::vector<int>>& C, 
                        int N) {
    // use AVX or SSE to multiply rows of A and columns of B in parallel
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; j += 8) { // loop in steps of 8 for AVX
            __m256i sum = _mm256_setzero_si256(); // initialize sum to 0

            // multiply a row of A by a column of B
            for (int k = 0; k < N; ++k) {
                __m256i row = _mm256_set1_epi32(A[i][k]); // replicate A[i][k] in all 8 elements
                __m256i col = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&B[k][j])); // load a column of B
                sum = _mm256_add_epi32(sum, _mm256_mullo_epi32(row, col)); // accumulate the product
            }

            // store the result in C
            _mm256_storeu_si256(reinterpret_cast<__m256i*>(&C[i][j]), sum);
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

    // perform matrix multiplication with SIMD
    matrixMultiplySIMD(A, B, C, N);

    auto stop = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = stop - start; // timing result

    // output the time taken
    std::cout << "\nTime taken for matrix multiplication (SIMD): " << duration.count() << " ms\n";

    return 0;
}