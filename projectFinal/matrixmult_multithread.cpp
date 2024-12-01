#include <iostream>
#include <vector>
#include <chrono> // for high res clock
#include <thread> // for std::thread

// function to perform matrix multiplication (to be run by each thread)
void matrixMultiplyThread(const std::vector<std::vector<int>>& A, 
                          const std::vector<std::vector<int>>& B, 
                          std::vector<std::vector<int>>& C, 
                          int startRow, int endRow, int N) {
    for (int i = startRow; i < endRow; ++i) {
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

    // number of threads
    int numThreads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;

    // split the rows of the matrix equally among the threads
    for (int t = 0; t < numThreads; ++t) {
        int startRow = t * N / numThreads;
        int endRow = (t + 1) * N / numThreads;
        threads.push_back(std::thread(matrixMultiplyThread, std::ref(A), std::ref(B), std::ref(C), startRow, endRow, N));
    }

    // join all threads
    for (auto& th : threads) {
        th.join();
    }

    auto stop = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = stop - start; // timing result

    // output the time taken
    std::cout << "\nTime taken for matrix multiplication (multithreaded): " << duration.count() << " ms\n";

    return 0;
}