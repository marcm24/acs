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

The same results can be observed for other sparcities: <br />
![image](https://github.com/user-attachments/assets/b7ea2cf2-d9cf-4da4-a13b-fd6c86f7b7d7)

The decreased sparcities have varying results in terms of an increase and decrease in some times. In this case this holds as the sparcities are low enough that there are mostly the same amount of 0's within the matrices. Even with this, the program still is indexing the matrices and spends time accessing each matrix element. <br />

## Dense-Dense Matrix Multiplication
Below are the timing results of multiplying dense matrices (of density 70%) together. The sizes were decreased to save time upon running as the matrices would have over an hour to multiply together if they were sized larger. <br />

![image](https://github.com/user-attachments/assets/7564a522-ef37-4a9c-b89c-0d7db5447292) <br />
We can see that the time to run the multiplication increases as the size of the matrices increase. <br />

## Sparse-Sparse Matrix Multiplication
Below are the timing results of multiplying sparse matrices of different sizes with the same sparcity of 20%: <br />
![image](https://github.com/user-attachments/assets/85b19bb9-3f92-43c6-a451-6441ee43b313) <br />
We can see that the time to run the multiplication increases as the size of the matrices increase. The times are ever so slightly less than that of the dense matrices. As stated previously, the program still makes the accesses to the matrix locations and will take the time to do so regardless if the value is zero or non-zero. <br />

Below are the timing results of multiplying sparse matrices of the same sizes with varying sparcities: <br />
![image](https://github.com/user-attachments/assets/c89a46b5-e636-43ef-acfa-68efa48bef84) <br />
The first iteration had sparcities of 10% and 40%, the second with sparcities of 20% and 30%, and the final iteration with sparcities of 1% and 25%. In this case we can see that the normal iteration runs quickest on the lower sparcities, where the SIMD optimized multiplications run faster with larger sparcities. <br />
## Dense-Sparse Matrix Multiplication
Below are the timing results of multiplying dense and sparse matrices of different sizes with the same density of 70% and sparcity of 30%: <br />
![image](https://github.com/user-attachments/assets/a042e15d-20bd-47c2-967f-2a8159d8a641) <br />

We can see that the time to run the multiplication increases as the size of the matrices increase. The timing is similar to that of the sparse-sparse multiplication and slightly quicker than the dense-dense multiplication. <br />

Below are the timing results of multiplying dense and sparse matrices of the same sizes with varying sparcities: <br />
![image](https://github.com/user-attachments/assets/d3afd4e8-0945-4e94-889d-778964651e48) <br />
The first iteration had density and sparcity of 90% and 10%, the second with density and sparcity of 80% and 20%, and the final iteration with 70% and 30%. In this case we can see the normal iteration speeds up as the density decreases and sparcity increases. With the optimization below the highest and lowest density iteration was the quickest. <br />

## Final Analysis and Conclusions
Throughout this project there were three optimization techniques applied to decrease the time it took for large matrix multiplication. These techniques were configurable multithreading, x86 SIMD instructions, and through cache optimization with cache blocking and loop unrolling. While all three of the optimization techniques decreased the time it took for the programs to run it was found that the cache optimization had the smallest impact, where multithreading and the SIMD instructions had greater impacts, with the SIMD instructions having the greatest impact on performance. The native matrix multiplication took hours to complete, with the optimization techniques speeding up the process. <br />

Throughout the optimization technique testing and further density/sparcity testing it was found to be that as the size of the matrices increased, the run time of the program drastically increased. We even reached times of up to over 3 hours with matrix sizes of 8000x8000, which left us with needing to use smaller matrix sizes for the sake of time. The sparcity and density of the matrices had little effect on the run time of the program. Although intuitively it would make sense that the sparse matrices would allow for faster program run time, with this programs implementation all the elements of the matrix are accessed. Since they are still accessed this will impact the run time. While the numbers were picked at random, the slight differences show in that the larger numbers, more frequently occurring in the dense matrices, will take longer to calculate than a matrix populated with mostly zeros. We can see that the sparse matrices throughout had slightly lower run times than the dense matrices. 
