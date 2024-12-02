#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
#include <stdlib.h>
#include <chrono>

// kernel for matrix multiplication
__global__ void matrixMultiplyKernel(const int* A, const int* B, int* C, int N) {
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (row < N && col < N) {
        int sum = 0;
        for (int k = 0; k < N; ++k) {
            sum += A[row * N + k] * B[k * N + col];
        }
        C[row * N + col] = sum;
    }
}

// helper function to handle CUDA errors
void checkCudaError(cudaError_t status, const char* message) {
    if (status != cudaSuccess) {
        fprintf(stderr, "%s: %s\n", message, cudaGetErrorString(status));
        exit(EXIT_FAILURE);
    }
}

int main() {
    const int N = 3000; // matrix dimension
    const int size = N * N * sizeof(int);

    // allocate memory for host matrices
    int* A = (int*)malloc(size);
    int* B = (int*)malloc(size);
    int* C = (int*)malloc(size);

    // initialize matrices A and B with random values
    for (int i = 0; i < N * N; ++i) {
        A[i] = rand() % 10;
        B[i] = rand() % 10;
    }

    // allocate memory for device matrices
    int* d_A, * d_B, * d_C;
    checkCudaError(cudaMalloc((void**)&d_A, size), "Failed to allocate device memory for A");
    checkCudaError(cudaMalloc((void**)&d_B, size), "Failed to allocate device memory for B");
    checkCudaError(cudaMalloc((void**)&d_C, size), "Failed to allocate device memory for C");

    // copy matrices A and B to device memory
    checkCudaError(cudaMemcpy(d_A, A, size, cudaMemcpyHostToDevice), "Failed to copy A to device");
    checkCudaError(cudaMemcpy(d_B, B, size, cudaMemcpyHostToDevice), "Failed to copy B to device");

    // Define block and grid sizes
    dim3 threadsPerBlock(16, 16);
    dim3 blocksPerGrid((N + threadsPerBlock.x - 1) / threadsPerBlock.x,
        (N + threadsPerBlock.y - 1) / threadsPerBlock.y);

    // measure the execution time of the kernel
    auto start = std::chrono::high_resolution_clock::now();

    // launch the kernel
    matrixMultiplyKernel << <blocksPerGrid, threadsPerBlock >> > (d_A, d_B, d_C, N);

    // synchronize to ensure kernel execution is complete
    checkCudaError(cudaDeviceSynchronize(), "Kernel execution failed");

    auto stop = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = stop - start;

    // copy result matrix C from device to host
    checkCudaError(cudaMemcpy(C, d_C, size, cudaMemcpyDeviceToHost), "Failed to copy C to host");

    // output the result matrix and execution time
  /*  printf("Result Matrix (C):\n");
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            printf("%d ", C[i * N + j]);
        }
        printf("\n");
    }*/
    printf("\nTime taken for matrix multiplication of size %d: %.3f ms\n", N, duration.count());

    // free device and host memory
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);
    free(A);
    free(B);
    free(C);

    return 0;
}
