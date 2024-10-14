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
*NOTE: For the sake of the optimization experiment the program structure was altered to run all of the tests.* <br />

The matrix multiplication performed in the previous section were repeated with the three optimization techniques. <br />
### Multithreading
After specifying 3 threads for the program to run on the following results were obtained: <br />
![image](https://github.com/user-attachments/assets/564bbaa7-3b7e-4a40-88ca-1071dbb7ea99)

The timing for the program to perform the multiplication significantly decreased, with a decrease from 12.1161 seconds to 3.9789 seconds for the 1000x1000 matrices, decrease from 2658.89 seconds (~44 minutes) to 801.343 seconds for the 5000x5000 matrices, and a decrease from 13249.3 seconds (~3.7 hours) to 3620.65 seconds (1 hour) for the 8000x8000 matrices. <br />

### x86 SIMD Instructions
The SIMD instructions implemented in the program process 8 integers in parallel. The 8 integers (256 bits) are stored into a register, and the packed integers are multiplied in two source registers. The values are then added and stored back into memory. The following results were obtained after utilizing the SIMD instructions: <br />
![image](https://github.com/user-attachments/assets/b951057a-e158-41c1-9405-40c65357219d)

The timing for the multiplication of matrices decreased more than that of with multithreading. The decrease in time was from 12.1161 seconds to 1.34683 seconds for the 1000x1000 matrices, decrease from 2658.89 seconds (~44 minutes) to 246.314 seconds (~4 minutes) for the 5000x5000 matrices, and a decrease from 13249.3 seconds (~3.7 hours) to 1127.87 seconds (~18.8 minutes) for the 8000x8000 matrices.

### Cache Optimization 
The cache was optimized in this program through two methods: cache blocking and loop unrolling. The matrix was broken up into smaller sections and blocked throughout the cache, to allow for quicker access to the elements of the matrices. The loops that are required to iterate through each layer of the matrix was unrolled by a factor of 4. The following results were obtained after utilizing the cache optimization techniques: <br />
![image](https://github.com/user-attachments/assets/e8769449-8d8e-4535-bcdd-9366b985625a)

We can see that the timing for the multiplication of the matrices is greater than the previous two optimization techniques, but still faster than the native implementation. The decrease in time was from 12.1161 seconds to 7.08713 seconds for the 1000x1000 matrices, decrease from 2658.89 seconds (~44 minutes) to 928.752 seconds (~15.4 minutes) for the 5000x5000 matrices, and a decrease from 13249.3 seconds (~3.7 hours) to 3913.36 seconds (~1.08 hours) for the 8000x8000 matrices. <br />
