#include <iostream>
#include <vector>
#include <chrono>
#include <immintrin.h> // for AVX
#include <cstdlib>     // for rand()

void convolution2D_SIMD(const std::vector<std::vector<int>>& input, 
                        const std::vector<std::vector<int>>& kernel,
                        std::vector<std::vector<int>>& output) {
    int kernelSize = kernel.size();
    int offset = kernelSize / 2;

    for (int i = offset; i < input.size() - offset; ++i) {
        for (int j = offset; j < input[i].size() - offset; ++j) {
            __m256i sum = _mm256_setzero_si256();
            for (int m = 0; m < kernelSize; ++m) {
                for (int n = 0; n < kernelSize; ++n) {
                    __m256i val = _mm256_set1_epi32(input[i + m - offset][j + n - offset]);
                    __m256i ker = _mm256_set1_epi32(kernel[m][n]);
                    sum = _mm256_add_epi32(sum, _mm256_mullo_epi32(val, ker));
                }
            }
            output[i][j] = _mm256_extract_epi32(sum, 0);
        }
    }
}

int main() {
    // set the size of the square matrix and kernel size
    int size = 3000;        // size of the square matrix 
    int kernelSize = 5;   // kernel size

    // initialize the input matrix with random values
    std::vector<std::vector<int>> input(size, std::vector<int>(size));
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            input[i][j] = rand() % 10; // random values between 0 and 9
        }
    }

    // initialize the kernel matrix with random values
    std::vector<std::vector<int>> kernel(kernelSize, std::vector<int>(kernelSize));
    for (int i = 0; i < kernelSize; ++i) {
        for (int j = 0; j < kernelSize; ++j) {
            kernel[i][j] = rand() % 3; // random values between 0 and 2
        }
    }

    // create the output matrix
    std::vector<std::vector<int>> output(size, std::vector<int>(size, 0));

    // start timing
    auto start = std::chrono::high_resolution_clock::now();
    
    // perform 2D convolution
    convolution2D_SIMD(input, kernel, output);
    
    // stop timing
    auto stop = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = stop - start; // timing result

    // print the input, kernel, and output matrices
    /*
    std::cout << "Input Matrix:\n";
    for (const auto& row : input) {
        for (const auto& val : row) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "\nKernel Matrix:\n";
    for (const auto& row : kernel) {
        for (const auto& val : row) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "\nOutput Matrix:\n";
    for (const auto& row : output) {
        for (const auto& val : row) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
    */
    // output the time taken for convolution
    std::cout << "\nTime taken for 2D convolution using x86 SIMD instructions: " << duration.count() << " ms\n";

    return 0;
}
