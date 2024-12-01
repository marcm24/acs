#include <iostream>
#include <vector>
#include <chrono>
#include <cstdlib> // for rand()

// Function for 2D convolution
void convolution2D(const std::vector<std::vector<int>>& input, 
                   const std::vector<std::vector<int>>& kernel,
                   std::vector<std::vector<int>>& output) {
    int kernelSize = kernel.size();
    int offset = kernelSize / 2;
    
    for (int i = offset; i < input.size() - offset; ++i) {
        for (int j = offset; j < input[i].size() - offset; ++j) {
            int sum = 0;
            for (int m = 0; m < kernelSize; ++m) {
                for (int n = 0; n < kernelSize; ++n) {
                    sum += input[i + m - offset][j + n - offset] * kernel[m][n];
                }
            }
            output[i][j] = sum;
        }
    }
}

int main() {
    int size = 2000;  // change this variable to control the matrix size
    int kernelSize = 3;  // kernel size for convolution, typically odd number

    // Generate a random input matrix of size x size
    std::vector<std::vector<int>> input(size, std::vector<int>(size));
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            input[i][j] = rand() % 10;  // Random values between 0 and 9
        }
    }

    // Generate a random kernel of kernelSize x kernelSize
    std::vector<std::vector<int>> kernel(kernelSize, std::vector<int>(kernelSize));
    for (int i = 0; i < kernelSize; ++i) {
        for (int j = 0; j < kernelSize; ++j) {
            kernel[i][j] = rand() % 3 - 1;  // Random values between -1 and 1 (e.g., for edge detection)
        }
    }

    // Output matrix to store results of the convolution
    std::vector<std::vector<int>> output(size, std::vector<int>(size, 0));

    // Perform the convolution and measure time
    auto start = std::chrono::high_resolution_clock::now();
    convolution2D(input, kernel, output);
    auto stop = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = stop - start;

    // Print the input matrix
    std::cout << "Input Matrix:" << std::endl;
    for (const auto& row : input) {
        for (const auto& val : row) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    // Print the kernel matrix
    std::cout << "\nKernel Matrix:" << std::endl;
    for (const auto& row : kernel) {
        for (const auto& val : row) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    // Print the output matrix
    std::cout << "\nOutput Matrix after Convolution:" << std::endl;
    for (const auto& row : output) {
        for (const auto& val : row) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    // Output the time taken for convolution
    std::cout << "\nTime taken for 2D convolution: " << duration.count() << " ms\n";

    return 0;
}
