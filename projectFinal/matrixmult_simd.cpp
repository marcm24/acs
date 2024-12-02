#include <iostream>
#include <vector>
#include <chrono>
#include <immintrin.h> // for AVX
#include <cstdlib>     // for rand()
#include <chrono>  // for high res clock

// function to utilize SIMD instruction for multiplying different rows
void simdMultiplyRow(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B, std::vector<std::vector<int>>& C, int startRow, int endRow) {
    int colsA = A[0].size();
    int colsB = B[0].size();

    for (int i = startRow; i < endRow; ++i) {
        for (int j = 0; j < colsB; ++j) {
            __m256i sum = _mm256_setzero_si256(); // initialize sum 

            for (int k = 0; k < colsA; k += 8) { // process 8 integers at a time
                // load 8 integers from each matrix into SIMD registers
                __m256i vecA = _mm256_loadu_si256((__m256i*)&A[i][k]);
                __m256i vecB = _mm256_loadu_si256((__m256i*)&B[k][j]);
                
                // multiply the values
                __m256i vecProduct = _mm256_mullo_epi32(vecA, vecB);
                
                // sum the results
                sum = _mm256_add_epi32(sum, vecProduct);
            }

            // extract sum from the SIMD register
            int tempSum[8];
            _mm256_storeu_si256((__m256i*)tempSum, sum);
            C[i][j] = tempSum[0] + tempSum[1] + tempSum[2] + tempSum[3] + tempSum[4] + tempSum[5] + tempSum[6] + tempSum[7];
        }
    }
}

// function to multiply matrices using SIMD
std::vector<std::vector<int>> multiplyMatricesSIMD(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B) {
    int rowsA = A.size();
    int colsA = A[0].size();
    int colsB = B[0].size();
    std::vector<std::vector<int>> result(rowsA, std::vector<int>(colsB, 0));

    simdMultiplyRow(A, B, result, 0, rowsA);

    return result;
}

// function to test the runtime
void runMultiplicationTimeSIMD(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B) {
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<std::vector<int>> C = multiplyMatricesSIMD(A, B);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "time for SIMD " << A.size() << "x" << B[0].size() << " matrix multiplication: " << duration.count() << " seconds" << std::endl;
}

int main() {
    const int N = 1000; // size of the matrices (e.g., 1024x1024)
    srand(static_cast<unsigned>(time(0))); // seed random number generator

    // initialize two N x N matrices with random integers between 0 and 100
    std::vector<std::vector<int>> A(N, std::vector<int>(N));
    std::vector<std::vector<int>> B(N, std::vector<int>(N));

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            A[i][j] = rand() % 101; // random numbers between 0 and 100
            B[i][j] = rand() % 101; // random numbers between 0 and 100
        }
    }

    // measure the time taken for SIMD matrix multiplication
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<std::vector<int>> C = multiplyMatricesSIMD(A, B);
    auto end = std::chrono::high_resolution_clock::now();

    // calculate duration
    std::chrono::duration<double> duration = end - start;
    std::cout << "time for SIMD " << N << "x" << N << " matrix multiplication: "
              << duration.count() << " seconds" << std::endl;

    return 0;
}