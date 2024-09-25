# Project 1: Cache and Memory Performance Profiling
## Introduction
The following information details research conducted on memory systems in modern computers, specifically the cache and main memory hierarchy.  <br />
## System Specifications
Experiments being conducted on the Ryzen 5 Pro 4650U with 6 cores. <br />
The cache sizes are as follows: <br />
CPU-Z Image (left) and WSL Image (right): <br />
![image](https://github.com/user-attachments/assets/f0884847-36a4-46af-9157-2fe2403abf6a)
![image](https://github.com/user-attachments/assets/3c163634-0f38-4e4b-951e-879b0190a957) <br />

L1 Cache Size: 32 KB (Per Core) [Total 192 KB] <br />
L2 Cache Size:  512 KB (Per Core) [Total 3.072 MB] <br />
L3 Cache Size: 4 MB (Shared between set of 2 cores) [Total 8 MB] <br />

Task Manager Image: <br />
![image](https://github.com/user-attachments/assets/fb6648dc-e666-481e-9154-c3694748da3f) <br />

The main memory size is 16GB. <br />

## Task 1: Read/Write Latency With Zero Queuing Delay
We are to test the read and write latency of the different memory systems with zero queuing delay. It is expected that the L1 cache will have the lowest latency, followed by the L2 cache, with L3 cache having the highest latency of the caches. The main memory will have the highest latency of all the memories as it is the largest and slowest. <br />

In the developed C++ program the read and write latencies for the various levels of memory are measured through the allocation of integers in vectors sized according to the memory under test. Each operation is timed to provide insight on latency. <br />

Upon runnning the [task1.cpp](https://github.com/marcm24/acs/blob/main/project1/task1.cpp) file we can see the output: <br />

![image](https://github.com/user-attachments/assets/37e01151-ac60-42ac-bb9d-d74cbabeae83) <br />
Confirming our expectations of the L1 cache having the lowest latency, followed by the L2 cache, L3 cache, and the main memory having the highest latency. <br />

## Task 2: Bandwidth Under Varying Access Patterns <br />

We are to test the memory bandwidth within various data access patterns. The data access granularity varies from 64B, 256B, and 1024B with different read/write ratios of read-only, write-only, 70:30 split, and 50:50 split. It is expected in this scenario that the bandwidth increases with higher data access granularities. We also expect to see more bandwidth with a higher read ratio due to the decreased overhead and complexity in a read operation vs. a write operation. <br />

In the developed C++ program read and write operations are performed on a 1GB buffer with the varying access patterns. The buffer is implemented to simulate memory usage, with the read and write operations occurring over multiple iterations to average out any variability in execution time and effectively calculate bandwidth. <br />

Upon running the [task2.cpp](https://github.com/marcm24/acs/blob/main/project1/task2.cpp) file we can see the output: <br />

![image](https://github.com/user-attachments/assets/cca4bcbe-6b02-4d9c-a429-9670a61e5ae3) <br />

We can confirm the validity of these bandwidths through the sysbench benchmarking tool, testing with the same granularities and memory buffer size. <br />

![image](https://github.com/user-attachments/assets/f326febc-268c-443a-a874-cea2900cc4a8) <br /> 

We can see that for a block size of 64B with a write only operation, we have 4922.04 / 1024 = 4.81, which is 0.51327 GB/s away from our discovered result but within a reasonable margin. Running the sysbench commands allow us to see the real bandwidth values: <br />
```
sysbench memory --memory-block-size=<block_size> --memory-total-size=1G run
```
## Task 3: Trade-off Between Read/Write Latency And Memory Throughput

## Task 4: Impact of Cach Miss Ratio on Software Speed

We are to test the impact of the cache miss ratio on the speed of our program running. It is expected that with a higher cache miss ratio the speed of our program will decrease. <br />

In the developed C++ program simple multiplication operations on integer elements within an array are performed with memory accesses at the current and next element for each iteration. The program simulates random memory accesses within the cache on varying sizes of memory blocks from 1KB to 20KB. <br />

Upon running the [task4.cpp](https://github.com/marcm24/acs/blob/main/project1/task4.cpp) file we can see the output: <br />

![image](https://github.com/user-attachments/assets/b55841a5-2fda-4f08-b48e-e262523ad552)

To simulate cache misses we can run cachegrind, a profiling tool apart of valgrind to simulate how programs interact with different levels of cache within the processor. We can run cachegrind on our program to simulate cache misses and analyze the new run times. We can run cachegrind with the following command: <br />

```
valgrind --tool=cachegrind ./task4.out
```
After running cachegrind we can examine the following output: <br />

![image](https://github.com/user-attachments/assets/e9b4f4b5-1def-450b-84c2-343c2c9aaf64) <br />

We can see a large increase in run time due to the simulated cache misses. With the L1 cache having a total miss ratio of 0.05% accross the L1 instruction and data cache, we can see an increase of ~35 times the original runtime. Although the times are in nanoseconds and still relatively quick in the human perspective, in terms of the machines performance this is a substantial increase in program run time. <br />

## Task 5: Impact of TLB Miss Ratio on Software Speed

## Technical Issues Encountered: 








