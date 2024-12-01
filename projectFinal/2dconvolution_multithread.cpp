#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <cstdlib> // For rand()

void convolution2D(const std::vector<std::vector<int>>& input, 
                   const std::vector<std::vector<int>>& kernel,
                   std::vector<std::vector<int>>& output, int startRow, int endRow) {
    int kernelSize = kernel.size();
    int offset = kernelSize / 2;

    for (int i = startRow; i < endRow; ++i) {
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
    // Set the size of the square matrix and kernel size
    int size = 10;        // Size of the square matrix (size x size)
    int kernelSize = 3;   // Kernel size (odd number for symmetry)
    int numThreads = 4;   // Number of threads to use for multithreading

    // Initialize the input matrix with random values
    std::vector<std::vector<int>> input(size, std::vector<int>(size));
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            input[i][j] = rand() % 10; // Random values between 0 and 9
        }
    }

    // Initialize the kernel matrix with random values
    std::vector<std::vector<int>> kernel(kernelSize, std::vector<int>(kernelSize));
    for (int i = 0; i < kernelSize; ++i) {
        for (int j = 0; j < kernelSize; ++j) {
            kernel[i][j] = rand() % 3; // Random values between 0 and 2
        }
    }

    // Create the output matrix
    std::vector<std::vector<int>> output(size, std::vector<int>(size, 0));

    // Calculate rows to be processed by each thread
    int rowsPerThread = size / numThreads;
    std::vector<std::thread> threads;

    // Start timing
    auto start = std::chrono::high_resolution_clock::now();
    
    // Launch threads for convolution computation
    for (int t = 0; t < numThreads; ++t) {
        int startRow = t * rowsPerThread;
        int endRow = (t == numThreads - 1) ? size : (t + 1) * rowsPerThread;
        threads.push_back(std::thread(convolution2D, std::ref(input), std::ref(kernel), std::ref(output), startRow, endRow));
    }

    // Wait for all threads to finish
    for (auto& th : threads) {
        th.join();
    }

    // Stop timing
    auto stop = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = stop - start; // timing result

    // Print the input, kernel, and output matrices
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

    // Output the time taken for convolution
    std::cout << "\nTime taken for 2D convolution using multithreading (" << numThreads << " threads): " << duration.count() << " ms\n";

    return 0;
}
