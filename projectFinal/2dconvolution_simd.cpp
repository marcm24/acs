#include <iostream>
#include <vector>
#include <chrono>
#include <immintrin.h> // for AVX
#include <cstdlib>     // for rand()

// function to perform 2D convolution using SIMD instructions
void convolution2D_SIMD(const std::vector<std::vector<int>>& input, 
                        const std::vector<std::vector<int>>& kernel,
                        std::vector<std::vector<int>>& output) {
    int kernelSize = kernel.size();
    int offset = kernelSize / 2;

    for (int i = offset; i < input.size() - offset; ++i) {
        for (int j = offset; j < input[i].size() - offset; ++j) {
            __m256i sum = _mm256_setzero_si256();
            for (int m = 0; m < kernelSize; ++m) {
                for (int n = 0; n < kernelSize; n += 8) {
                    // load 8 values from the input matrix and kernel
                    __m256i val = _mm256_loadu_si256((__m256i*)&input[i + m - offset][j + n - offset]);
                    __m256i ker = _mm256_loadu_si256((__m256i*)&kernel[m][n]);
                    sum = _mm256_add_epi32(sum, _mm256_mullo_epi32(val, ker));
                }
            }
            // store the summed value in the output matrix
            int tempSum[8];
            _mm256_storeu_si256((__m256i*)tempSum, sum);
            output[i][j] = tempSum[0] + tempSum[1] + tempSum[2] + tempSum[3] +
                           tempSum[4] + tempSum[5] + tempSum[6] + tempSum[7];
        }
    }
}

// function to initialize a matrix with random integers in a specified range
void initializeMatrix(std::vector<std::vector<int>>& matrix, int minVal, int maxVal) {
    for (auto& row : matrix) {
        for (auto& val : row) {
            val = rand() % (maxVal - minVal + 1) + minVal;
        }
    }
}

// function to test the runtime of 2D convolution
void runConvolutionTimeSIMD(const std::vector<std::vector<int>>& input, 
                            const std::vector<std::vector<int>>& kernel, 
                            std::vector<std::vector<int>>& output) {
    auto start = std::chrono::high_resolution_clock::now();
    convolution2D_SIMD(input, kernel, output);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> duration = end - start;
    std::cout << "\nTime taken for 2D convolution using x86 SIMD instructions: "
              << duration.count() << " ms\n";
}

int main() {
    const int size = 3000;       // size of the square matrix
    const int kernelSize = 5;    // kernel size

    // initialize the input and kernel matrices
    std::vector<std::vector<int>> input(size, std::vector<int>(size));
    std::vector<std::vector<int>> kernel(kernelSize, std::vector<int>(kernelSize));
    std::vector<std::vector<int>> output(size, std::vector<int>(size, 0));

    initializeMatrix(input, 0, 9);   // random values between 0 and 9
    initializeMatrix(kernel, 0, 2);  // random values between 0 and 2

    // test the runtime of SIMD convolution
    runConvolutionTimeSIMD(input, kernel, output);

    return 0;
}