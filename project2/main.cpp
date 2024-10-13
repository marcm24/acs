#include <iostream>
#include <cstdlib>
#include <ctime>  
#include <vector>
#include <string>
#include <chrono> // high res clock
#include <thread> // multithreading
#include <immintrin.h> // for x86 SIMD instructions

int BLOCK_SIZE = 32; // cache size

// dense matrix generation
std::vector<std::vector<int>> denseMatrix(int rows, int cols, double density) {
    std::vector<std::vector<int>> matrix(rows, std::vector<int>(cols, 0));

    // random number generation
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            // random pick of 0 or a number between 1 and 10
            double randNum = static_cast<double>(std::rand()) / RAND_MAX;

            // fill matrix based on threshold
            if (randNum < density) {
                // generate a random number between 1 and 10 for non-zero values
                matrix[i][j] = std::rand() % 10 + 1; // Generates a number from 1 to 10
            }
        }
    }

    return matrix;
}

std::vector<std::vector<int>> sparceMatrix(int rows, int cols, double sparcity) {
    std::vector<std::vector<int>> matrix(rows, std::vector<int>(cols, 0));

    // random number generation
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            // random pick of 0 or a number between 1 and 10
            double randNum = static_cast<double>(std::rand()) / RAND_MAX;

            // fill matrix based on threshold
            if (randNum < sparcity) {
                // generate a random number between 1 and 10 for non-zero values
                matrix[i][j] = std::rand() % 10 + 1; // Generates a number from 1 to 10
            }
        }
    }

    return matrix;
}

// function to multiply two matrices
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

void runMultiplicationTime(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B) {
    int dimensions = A.size();

    auto start = std::chrono::high_resolution_clock::now();

    // if multiplication of matrices is not possible
    try {
        std::vector<std::vector<int>> C = multiplyMatrices(A, B);
    } catch (const std::invalid_argument& e) {
        std::cerr << "error: " << e.what() << std::endl;
        return;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "base time for " << dimensions << "x" << dimensions << " matrix multiplicaton: " <<  duration.count() << " seconds" << std::endl;
}

// function for multiplying certain sections of matrix
void multiplySection(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B, 
                     std::vector<std::vector<int>>& C, int startRow, int endRow) {
    int colsA = A[0].size();
    int colsB = B[0].size();

    for (int i = startRow; i < endRow; ++i) {
        for (int j = 0; j < colsB; ++j) {
            for (int k = 0; k < colsA; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// function to multiply matrices using multithreading
std::vector<std::vector<int>> multiplyMatricesMT(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B, int numThreads) {
    int rowsA = A.size();
    int colsA = A[0].size();
    int colsB = B[0].size();
    std::vector<std::vector<int>> result(rowsA, std::vector<int>(colsB, 0));

    std::vector<std::thread> threads;
    int rowsPerThread = rowsA / numThreads;
    int remainingRows = rowsA % numThreads;

    // launch threads to handle different sections of the matrix
    for (int i = 0; i < numThreads; ++i) {
        int startRow = i * rowsPerThread;
        int endRow = (i + 1) * rowsPerThread + (i == numThreads - 1 ? remainingRows : 0);
        threads.emplace_back(multiplySection, std::cref(A), std::cref(B), std::ref(result), startRow, endRow);
    }

    // join threads
    for (auto& thread : threads) {
        thread.join();
    }

    return result;
}

// function to test the runtime of multithreading
void runMultiplicationTimeMT(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B, int numThreads) {
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<std::vector<int>> C = multiplyMatricesMT(A, B, numThreads);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "time for " << A.size() << "x" << B[0].size() << " matrix multiplication with " << numThreads << " threads: " << duration.count() << " seconds" << std::endl;
}

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

// block sections of matrix throughout cache
std::vector<std::vector<int>> multiplyMatricesBlocked(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B) {
    int n = A.size();
    std::vector<std::vector<int>> C(n, std::vector<int>(n, 0));

    // iterate through blocks
    for (int ii = 0; ii < n; ii += BLOCK_SIZE) {
        for (int jj = 0; jj < n; jj += BLOCK_SIZE) {
            for (int kk = 0; kk < n; kk += BLOCK_SIZE) {
                // perform multiplication on each block
                for (int i = ii; i < std::min(ii + BLOCK_SIZE, n); ++i) {
                    for (int j = jj; j < std::min(jj + BLOCK_SIZE, n); ++j) {
                        int sum = 0;
                        int k = kk;

                        // loop unrolling
                        for (; k <= std::min(kk + BLOCK_SIZE, n) - 4; k += 4) {
                            sum += A[i][k] * B[k][j];
                            sum += A[i][k + 1] * B[k + 1][j];
                            sum += A[i][k + 2] * B[k + 2][j];
                            sum += A[i][k + 3] * B[k + 3][j];
                        }

                        // handle remaining elements
                        for (; k < std::min(kk + BLOCK_SIZE, n); ++k) {
                            sum += A[i][k] * B[k][j];
                        }

                        C[i][j] += sum;
                    }
                }
            }
        }
    }

    return C;
}

// function to measure and print the runtime
void runMultiplicationTimeBlocked(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B) {
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<std::vector<int>> C = multiplyMatricesBlocked(A, B);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "Time for blocked " << A.size() << "x" << B[0].size() << " matrix multiplication: " << duration.count() << " seconds" << std::endl;
}

int main(int argc, char *argv[]) {

// error checking if number of command line arguments not sufficient
try {
    if (argc < 2) {
        throw std::runtime_error("error: expected specification for optimization technique");
    }
}
catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return 1;
} 

std::string optimization = argv[1]; // optimization technique string

// matrix generation
std::vector<std::vector<int>> matrixA = sparceMatrix(100, 100, 0.1);
std::vector<std::vector<int>> matrixB = sparceMatrix(100, 100, 0.1);
std::vector<std::vector<int>> matrixC = sparceMatrix(1000, 1000, 0.1);
std::vector<std::vector<int>> matrixD = sparceMatrix(1000, 1000, 0.1);

if (optimization == "mt") { // mt for multiple threads
    if (argc < 3){
        std::cerr << "error: expected specification of amount of threads" << std::endl;
    } else {
        int threads = std::stoi(argv[2]);
        runMultiplicationTime(matrixA, matrixB);
        runMultiplicationTime(matrixC, matrixD);
        std::cout << "--------------------------------------------------------" << std::endl;
        runMultiplicationTimeMT(matrixA, matrixB, threads);
        runMultiplicationTimeMT(matrixC, matrixD, threads);
    }
    } else if (optimization == "simd") { // simd for x86 instructions | compile with -march=native
        runMultiplicationTime(matrixA, matrixB);
        runMultiplicationTime(matrixC, matrixD);
        std::cout << "--------------------------------------------------------" << std::endl;
        runMultiplicationTimeSIMD(matrixA, matrixB);
        runMultiplicationTimeSIMD(matrixC, matrixD);
    } else if (optimization == "cache") { // cache to minimize cache misses
        runMultiplicationTime(matrixA, matrixB);
        runMultiplicationTime(matrixC, matrixD);
        std::cout << "--------------------------------------------------------" << std::endl;
        runMultiplicationTimeBlocked(matrixA, matrixB);
        runMultiplicationTimeBlocked(matrixC, matrixD);
    } else {
        std::cerr << "error: unsupported optimization technique" << std::endl; // if command argument is not properly input
    }

    return 0;
}