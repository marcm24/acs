# Final Project: Program Acceleration With NVIDIA CUDA

## Introduction
In previous projects we were able to observe the performance benefits of better utilizing the CPU (central processing unit) hardware resources with multithreading and x86 SIMD instructions. As we have learned CPUs are designed for a wide range of general-purpose tasks making them very versatile. However, this versatility has the drawback of lack of specialization where the CPU may not be able to efficiently perform a single type of task. On the other hand, GPUs (graphics processing unit) are not designed for a wide range of tasks and are able to perform 1 task, being parallel processing. In this project we will be utilizing the NVIDIA CUDA toolkit to execude code directly using the GPU. Various tests throughout programs will be performed to understand the benefits of utilizing a GPU in specific applications.

If interested, see the presentation for the project [here](https://docs.google.com/presentation/d/1mqr0XEmGRDKSmAwHjD3zZ_aDUSLbXm8pjlxnAqnYrqY/edit?usp=sharing)

## System Specifications + Requirements

Tests performed using the following system specifications: <br />
![image](https://github.com/user-attachments/assets/0de8c52b-3500-4317-957d-b4cd971ad9f1) <br />
The full list of system specifcations with detailed information is provided in the [system_specs.txt](https://github.com/marcm24/acs/blob/main/projectFinal/system_specs.txt) file. <br />
The use of the NVIDIA CUDA toolkit requires a CUDA capable NVIDIA GPU to be installed on the system the program is being run on. The CUDA toolkit also requires Microsoft Visual Studio to be installed on the system to utilize some of the software components such as the cl compiler tool. More detailed requirements for the NVIDIA CUDA toolkit is listed in section 1.1 of the [NVIDIA CUDA Installation Guide for
Microsoft Windows](https://developer.download.nvidia.com/compute/cuda/11.4.0/docs/sidebar/CUDA_Installation_Guide_Windows.pdf). <br />

## Test 1: Vector Addition
The first test we will use to analyze the effects on program time utilizing a GPU for processing is through vector addition. Vector addition is applied in many programs and in theory can be accelerated with the use of the parallel architecture of a GPU. The following results are obtained for the base [C++](https://github.com/marcm24/acs/blob/main/projectFinal/vectoradd.cpp) program: <br />
![image](https://github.com/user-attachments/assets/d8e25545-1a7e-4eb4-95b6-5ae587788441) <br />

We can see that it runs in 0.42ms for vector sizes of 100000 elements. The results for the [CUDA program](https://github.com/marcm24/acs/blob/main/projectFinal/vectoradd.cu) are shown below: <br />
![image](https://github.com/user-attachments/assets/500f34a9-5ef5-4793-9ab5-89d61dcf0cbd) <br />
We can see that it runs in 151.0616ms for vector sizes of 100000 elements. <br />

The following results for the vector addition on the C++ program for vector sizes of 500000 are shown below: <br />
![image](https://github.com/user-attachments/assets/89da2dd7-24e1-45df-8a44-9132fcc0a2e0) <br />
This program runs in 2.1375ms. <br />


The following results for the CUDA program with vector sizes of 500000 are shown below: <br />
![image](https://github.com/user-attachments/assets/7971d45f-69e9-4e27-b4be-7533e2608786) <br />
We can see that it runs in 140.293ms. <br />

The following results for the C++ program for vector sizes of 1000000 are shown below: <br />
![image](https://github.com/user-attachments/assets/8edb2439-9d93-4f79-8139-d2a4caebd6ca) <br />
We can see that it runs in 4.2138ms. <br />

The following results for the CUDA program with vector sizes of 1000000 are shown below: <br />
![image](https://github.com/user-attachments/assets/1798c938-24d6-40f9-99a6-92d4aaaf1884) <br />
We can see that it runs in 155.9924ms. <br />

From these tests we can see that the C++ program ran faster, but as the vector sizes increased the time the program ran increased. The CUDA program took longer, but regardless of the vector size the run time remained in the same range of 140ms-160ms.
## Test 2: Matrix Multiplication
The next test we will use to analyze the effects on program time utilizing a GPU for processing is through matrix multiplication. Matrix multiplication is utilized in programs for various applications, such as image processing, gaming, machine learning, etc. <br />

In this test we will not only compare the results of the [matrix multiplication CUDA program](https://github.com/marcm24/acs/blob/main/projectFinal/matrixmult.cu) to a [base C++ program](https://github.com/marcm24/acs/blob/main/projectFinal/matrixmult.cpp), but with a [multithreaded C++ program](https://github.com/marcm24/acs/blob/main/projectFinal/matrixmult_multithread.cpp) and a [C++ program using x86 SIMD instructions](https://github.com/marcm24/acs/blob/main/projectFinal/matrixmult_simd.cpp). <br />

From results obtained in previous projects matrix multiplication of matrices sized 5000x5000 or greater took, multiple hours to complete. In the interest of time we will perform the base operation on a matrix sized at most 3000x3000. For reference, 8000x8000 matrices took 3 hours to multiply on a base program, and 1 hour with multithreading. <br />

### Base C++ Program
The following results were obtained for a base matrix multiplication of matrices sized 1000x1000, 2000x2000, and 3000x3000: <br />
![image](https://github.com/user-attachments/assets/9900d779-b898-431e-91bd-827025cd0a47) <br />
![image](https://github.com/user-attachments/assets/ac82bd07-4c54-4a01-92e3-c237f8c2f157) <br />
![image](https://github.com/user-attachments/assets/b7d0062e-5265-4bc9-8be2-21dd2fa06d12) <br />

The 1000x1000 matrix took 11140.4ms, or 11s to multiply, the 2000x2000 matrix 137259ms, or 2 minutes and 17s to multiply, and the 3000x3000 matrix took 504736ms to mutliply, or 8 minutes and 24s. <br />

### Multithreaded C++ Program

The following results were ovtained for a multithreaded matrix multiplation of matricies sized 1000x1000, 2000x2000, and 3000x3000: <br />
![image](https://github.com/user-attachments/assets/19db2e22-0a48-4921-b39a-7f4193fb1416) <br />
![image](https://github.com/user-attachments/assets/a0aa8917-5913-421d-99e0-9053aac21a21) <br />
![image](https://github.com/user-attachments/assets/85dd9554-372b-42a9-ab21-cf07312d72f5) <br />

The 1000x1000 matrix took 1483.25ms, or 1.4s to multiply, the 2000x2000 matrix 17098.9ms, or 17s to multiply, and the 3000x3000 matrix took 65384.5ms to mutliply, or 1 minute and 5s. We can see that the multithreaded program with the maximum amount of threads allowable by the CPU (8 for this systems i7-9700K) already increased the runtime of the program. <br />

### C++ Program Using x86 SIMD Instructions
(Note: For compilation the command *-march=native* must be included to utilize the x86 SIMD instructions)
The following results were ovtained for a matrix multiplation program utilizing x86 SIMD instructions of matricies sized 1000x1000, 2000x2000, and 3000x3000: <br />

![image](https://github.com/user-attachments/assets/1250fb88-52b2-43b6-bbc7-bb165ec68757) <br />
![image](https://github.com/user-attachments/assets/1464c699-00be-468b-9a82-74ec2375a6d8) <br />
![image](https://github.com/user-attachments/assets/0780c5d2-ba73-4b60-8b0c-c0fb81e0120e) <br />

The 1000x1000 matrix took 1363.04ms or, 1.36s to multiply, the 2000x2000 matrix 12756.9ms or, 12.7s to multiply, and the 3000x3000 matrix took 42590.6ms or, 42.59s. We can see that the program x86 SIMD instructions increased the runtime of the program more than the previous implementations. <br />

### CUDA Program
![image](https://github.com/user-attachments/assets/fbd2bc2a-1178-4b35-87e2-8ea2a9c8bb42) <br />
![image](https://github.com/user-attachments/assets/57e33fce-d812-48ae-aa14-4909d5c174a0) <br />
![image](https://github.com/user-attachments/assets/ed2f8e04-9e83-4315-8ebf-0e721811e8f9) <br />

The 1000x1000 matrix took 15.773ms to multiply, the 2000x2000 matrix 134.443ms to multiply, and the 3000x3000 matrix took 416.962ms to mutliply. This is significantly faster than any of the other programs run previously. <br />

For the sake of example, we can run a large test with matrices sized 10,000x10,000 with the CUDA program: <br />
![image](https://github.com/user-attachments/assets/eb4ec00d-7680-4d33-a8b8-0a5ce6c9a4ac) <br />
This took 14212.663ms to run, or 14 seconds. Running this test on any of the previous programs would have taken hours to run. As shown in [project 2](https://github.com/marcm24/acs/blob/main/project2/writeup.md). <br />

## Test 3: Two-Dimensional Convolution
2D convolution is a technique used in image processing for image filtering, edge detection, sharpening, and blurring. It involves taking an image, represented as a large 2D matrix and applying a kernel, represented as a small matrix over the image to modify the pixels of the image. The mathematical operation of the technique is shown below, sourced from: [Example of 2D Convolution](https://www.songho.ca/dsp/convolution/convolution2d_example.html). <br />

![image](https://github.com/user-attachments/assets/52ccfcb3-f5e2-470c-ae34-35f14b481b73) <br />

The operation was performed in a [base C++ program](https://github.com/marcm24/acs/blob/main/projectFinal/2dconvolution.cpp), [multithreaded C++ program](https://github.com/marcm24/acs/blob/main/projectFinal/2dconvolution_multithread.cpp), [C++ program using x86 SIMD instructions](https://github.com/marcm24/acs/blob/main/projectFinal/2dconvolution_simd.cpp), and a [CUDA program](https://github.com/marcm24/acs/blob/main/projectFinal/2dconvolution.cu).

Each of the convolution programs performed the operation on an image of size 3000x3000, with kernel size 5x5. <br />
### Base C++ Program
![image](https://github.com/user-attachments/assets/644500bc-42b8-4e92-b18d-9a0b9e880e22) <br />
The base program took 1574.39ms, or 1.5 seconds to complete. <br />
### Multithreaded C++ Program
![image](https://github.com/user-attachments/assets/9b47a1f5-868b-4c7c-97a1-bf11a12703a1) <br />
The multithreaded program took 356.13ms to complete. <br />
### C++ Program Using x86 SIMD Instructions
![image](https://github.com/user-attachments/assets/96206948-7238-40d9-bf9c-285d9259ce06) <br />
The C++ program using x86 SIMD instructions took 555.399ms, or 0.55 seconds to complete. <br />
### CUDA Program
![image](https://github.com/user-attachments/assets/06de6ba6-bd92-4b52-8571-59bdf89c490c) <br />
The CUDA program took 191.413ms to complete. <br />

We can see that the multithreaded program was faster than the base and program using SIMD instructions however, the CUDA program was faster than all of these programs.

## Analysis
### Vector Addition
The results showed that the base C++ program runtime was much faster than that of the CUDA program. The graph of the results is shown below: <br />

![image](https://github.com/user-attachments/assets/ed84d569-f607-4dee-b250-a33bfbbce31f) <br />

We can notice that as we increase the size of the vector, the base C++ program runtime increases by a large factor where the CUDA program runtime stays largely stable. Due to the single for loop in the C++ program, we expect the runtime to increase with the larger input size. CUDA's use of the GPU for parallel programming might not show validating results with smaller data sizes due to the memory overhead, however we can see that the time is remaining within the range of 140ms-160ms illustrating that the GPU can still benefit the runtime of our program by making it a constant time, and eventually lower then up to when the base program would eventually catch up to the runtime of the CUDA program. <br />

### Matrix Multiplication
The results showed that the CUDA program was able to multiply the matrices the fastest, followed by the SIMD program, multithreaded program and then finally the base program. The results are shown in the plot below: <br />

![image](https://github.com/user-attachments/assets/6a707c5d-6c82-4278-925b-417aa98ba0cc) <br />

As expected, the CUDA program has the fastest runtime. The base program takes the longest without any optimization. Any form of optimization used would be substantially better than without it, however CUDA is able to perform the multiplication the best, especially for much larger matrices. <br />

### Two-Dimensional Convolution
The results again showed the CUDA program with the fastest runtime. Due to the more applied nature of the image convolution, we can see the GPUs specific architecture excel in this operation. The plot of runtimes are shown below: <br />

 ![image](https://github.com/user-attachments/assets/7f8c7569-e4a9-4deb-abda-b5dc2b2955a7) <br />

 In this test the multithreaded program was able to operate faster than the SIMD program. The smaller size of the kernel matrix might cause resource overhead where there are many populated registers that are waiting to use their data to multiply with. With this we can draw that the SIMD instructions work best with the registers full, or with similarly sized data. The GPU was able to handle this arithmetic well in comparison, and was able to perform faster. <br />

In conclusion we can see the benefits of utilizing a GPU in programs, especially specific applications requiring heavy arithmetic. We observed that better utilizing the CPU resources for our programs makes them faster, but with a more parallel architecture we are able to perform arithmetic at a much faster rate. <br />
