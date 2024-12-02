#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#include <chrono> // for timing
#include <stdlib.h> // for rand()

// helper function for using CUDA to perform 2D convolution on a matrix
cudaError_t convolutionWithCuda(int* output, const int* input, const int* kernel, int matrixSize, int kernelSize);

// CUDA kernel for performing 2D convolution
__global__ void convolutionKernel(int* output, const int* input, const int* kernel, int matrixSize, int kernelSize)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    int j = blockIdx.y * blockDim.y + threadIdx.y;

    if (i < matrixSize && j < matrixSize) {
        int offset = kernelSize / 2;
        int sum = 0;

        // Perform convolution operation
        for (int m = -offset; m <= offset; ++m) {
            for (int n = -offset; n <= offset; ++n) {
                int row = i + m;
                int col = j + n;
                if (row >= 0 && row < matrixSize && col >= 0 && col < matrixSize) {
                    sum += input[row * matrixSize + col] * kernel[(m + offset) * kernelSize + (n + offset)];
                }
            }
        }
        output[i * matrixSize + j] = sum;
    }
}

int main()
{
    int matrixSize = 3000; // set the matrix size
    int kernelSize = 5;    // set the kernel size (odd numbers like 3, 5, 7 are typical)

    // define input matrix and kernel
    int* input = (int*)malloc(matrixSize * matrixSize * sizeof(int));
    int* kernel = (int*)malloc(kernelSize * kernelSize * sizeof(int));
    int* output = (int*)malloc(matrixSize * matrixSize * sizeof(int));

    // initialize input matrix with random values
    for (int i = 0; i < matrixSize * matrixSize; ++i) {
        input[i] = rand() % 100; // Random values between 0 and 99
    }

    // initialize kernel matrix with random values
    for (int i = 0; i < kernelSize * kernelSize; ++i) {
        kernel[i] = rand() % 3; // Random values between 0 and 2 for simplicity
    }

    // start timing
    auto start = std::chrono::high_resolution_clock::now();

    // perform convolution using CUDA
    cudaError_t cudaStatus = convolutionWithCuda(output, input, kernel, matrixSize, kernelSize);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "convolutionWithCuda failed!\n");
        return 1;
    }

    // end timing
    auto stop = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = stop - start;

    // print output matrix for small sizes (optional)
    if (matrixSize <= 10) {
        printf("Input Matrix:\n");
        for (int i = 0; i < matrixSize; ++i) {
            for (int j = 0; j < matrixSize; ++j) {
                printf("%d ", input[i * matrixSize + j]);
            }
            printf("\n");
        }
        printf("\nOutput Matrix (after convolution):\n");
        for (int i = 0; i < matrixSize; ++i) {
            for (int j = 0; j < matrixSize; ++j) {
                printf("%d ", output[i * matrixSize + j]);
            }
            printf("\n");
        }
    }
    else {
        printf("Convolution completed for matrix size: %d\n", matrixSize);
    }

    // Output the time taken for convolution
    printf("Time taken for 2D convolution: %f ms\n", duration.count());

    // Free memory
    free(input);
    free(kernel);
    free(output);

    return 0;
}

// helper function for using CUDA to perform 2D convolution
cudaError_t convolutionWithCuda(int* output, const int* input, const int* kernel, int matrixSize, int kernelSize)
{
    int* dev_input = 0;
    int* dev_kernel = 0;
    int* dev_output = 0;
    cudaError_t cudaStatus;

    // choose which GPU to run on (change this on a multi-GPU system)
    cudaStatus = cudaSetDevice(0);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaSetDevice failed! Do you have a CUDA-capable GPU installed?\n");
        return cudaStatus;
    }

    // allocate GPU buffers for input, kernel, and output matrices
    cudaStatus = cudaMalloc((void**)&dev_input, matrixSize * matrixSize * sizeof(int));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed for input matrix!\n");
        return cudaStatus;
    }

    cudaStatus = cudaMalloc((void**)&dev_kernel, kernelSize * kernelSize * sizeof(int));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed for kernel!\n");
        return cudaStatus;
    }

    cudaStatus = cudaMalloc((void**)&dev_output, matrixSize * matrixSize * sizeof(int));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed for output matrix!\n");
        return cudaStatus;
    }

    // copy input and kernel matrices from host memory to GPU buffers
    cudaStatus = cudaMemcpy(dev_input, input, matrixSize * matrixSize * sizeof(int), cudaMemcpyHostToDevice);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed for input matrix!\n");
        return cudaStatus;
    }

    cudaStatus = cudaMemcpy(dev_kernel, kernel, kernelSize * kernelSize * sizeof(int), cudaMemcpyHostToDevice);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed for kernel matrix!\n");
        return cudaStatus;
    }

    // launch kernel with grid and block sizes based on matrix size
    dim3 blockSize(16, 16);
    dim3 gridSize((matrixSize + blockSize.x - 1) / blockSize.x, (matrixSize + blockSize.y - 1) / blockSize.y);

    convolutionKernel << <gridSize, blockSize >> > (dev_output, dev_input, dev_kernel, matrixSize, kernelSize);

    // check for errors during kernel launch
    cudaStatus = cudaGetLastError();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "convolutionKernel launch failed: %s\n", cudaGetErrorString(cudaStatus));
        return cudaStatus;
    }

    // synchronize device
    cudaStatus = cudaDeviceSynchronize();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaDeviceSynchronize returned error code %d after launching convolutionKernel!\n", cudaStatus);
        return cudaStatus;
    }

    // copy output matrix from GPU buffer to host memory
    cudaStatus = cudaMemcpy(output, dev_output, matrixSize * matrixSize * sizeof(int), cudaMemcpyDeviceToHost);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed for output matrix!\n");
        return cudaStatus;
    }

    // free GPU memory
    cudaFree(dev_output);
    cudaFree(dev_input);
    cudaFree(dev_kernel);

    return cudaSuccess;
}
