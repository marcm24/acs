#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <cstdlib> // for rand()

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
                    // Ensure the indices are within bounds
                    int row = i + m - offset;
                    int col = j + n - offset;
                    if (row >= 0 && row < input.size() && col >= 0 && col < input[i].size()) {
                        sum += input[row][col] * kernel[m][n];
                    }
                }
            }
            output[i][j] = sum;
        }
    }
}

int main() {
    // set the size of the square matrix and kernel size
    int size = 3000;        // size of the square matrix (size x size)
    int kernelSize = 5;   // kernel size (odd number for symmetry)
    int numThreads = std::thread::hardware_concurrency();   // number of threads to use for multithreading (max cpu can give)

    // initialize the input matrix with random values
    std::vector<std::vector<int>> input(size, std::vector<int>(size));
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            input[i][j] = rand() % 10; // Random values between 0 and 9
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

    // calculate rows to be processed by each thread
    int rowsPerThread = size / numThreads;
    std::vector<std::thread> threads;

    // start timing
    auto start = std::chrono::high_resolution_clock::now();
    
    // launch threads for convolution computation
    for (int t = 0; t < numThreads; ++t) {
        int startRow = t * rowsPerThread;
        int endRow = (t == numThreads - 1) ? size : (t + 1) * rowsPerThread;
        threads.push_back(std::thread(convolution2D, std::ref(input), std::ref(kernel), std::ref(output), startRow, endRow));
    }

    // wait for all threads to finish
    for (auto& th : threads) {
        th.join();
    }

    // stop timing
    auto stop = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = stop - start; // timing result

    // output the time taken for convolution
    std::cout << "\nTime taken for 2D convolution using multithreading (" << numThreads << " threads): " << duration.count() << " ms\n";

    return 0;
}
