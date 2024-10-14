# Project 2: Dense/Sparse Matrix-Matrix Multiplication
## Introduction
This project details the effects of multithreading, single instruction multiple data (SIMD) instructions, and cache optimization on multiplying large densely and sparsely populated matrices. For reference, a dense matrix is a matrix where there are more non-zero elements than zero elements. A sparce matrix is a matrix in which there are more zero elements than non-zero elements. <br />
## Program Run Instructions
For program compilation: <br />
```
g++ main.cpp -o mult.out -march=native
```
The three different optimization techniques outlined in the introduction can be used through modification of the first command line argument after the file name. <br />

For multithreading, specify "mt" and the number of threads after: <br />
```
./mult.out mt 3
```
For SIMD instructions, specify "simd": <br />
```
./mult.out simd
```
For cache optimization, specify "cache": <br />
```
./mult.out cache
```
## Native Matrix Multiplication
The base time for the matrix multiplication is below: <br />
![image](https://github.com/user-attachments/assets/26fee3af-5627-4624-8e80-49cf44880a65) <br />

The matrix sizes being operated on are 1000x1000, 5000x5000, and 8000x8000. Each matrix is sparse, with sparcity of 10%. Converted to a more understandable time, the 5000x5000 matrix multiplication took 44 minutes and the 8000x8000 matrix multiplication took 3.7 hours. As we can see in the above, the largest matrix size took a very long time to operate on. We were directed to use a smaller matrix size (smaller than 10,000x10,000) if the run time would be too long. The 8000x8000 already took long, and a size of 10,000x10,000 would have taken much longer making testing extremely difficult. <br />

## Optimization Experiments
