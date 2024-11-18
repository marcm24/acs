# Project 4: Implementation of Dictionary Codec

## Introduction
This project implements a dictionary codec. Given a large file (>1GB), we must parse the contents of this file into an encoded file by storing these values within a dictionary. This encoding will make operations with the contents of this file much more effecient. Timing results will be recorded and analyzed in this project. <br />

## Program Run Instructions
There are two .cpp files to perform these tests. The [encoder_test.cpp](https://github.com/marcm24/acs/blob/main/project4/encoder_test.cpp) file allows for the testing how long it takes to encode the given file with a baseline test, and multithreaded test. The [query_test.cpp](https://github.com/marcm24/acs/blob/main/project4/query_test.cpp) file allows for the search of specific strings and substrings within the encoded dictionary, with a baseline test and x86 SIMD test for timing results. <br />

To compile the program for testing the speed of encoding the file: <br />
```
g++ encoder_test.cpp -o encoder.out -march=native
```
To compile the program for testing the speed of querying the file: <br />
```
g++ query_test.cpp -o query.out -march=native
```
The query test program will run continuously unless the user inputs the word "exit" to exit the program. <br />
## Dictionary Encoding
With the execution of the [encoder_test.cpp](https://github.com/marcm24/acs/blob/main/project4/encoder_test.cpp) file, we can perform a baseline test to see how long it will take to allocate the contents of the given text file, and compare this with the multithreaded implementaiton. <br />

The program is designed to use all available threads that the CPU can offer, where in the initial test it is 8 threads: <br />

![image](https://github.com/user-attachments/assets/dc784352-f1c3-4ef9-a347-87d9cdcf9db9) <br />

Here we can see that the baseline encoding takes approximately 145 seconds or 2 minutes and 25 seconds. When utilizing 8 threads concurrently, we can see an improvement to just 48.0625 seconds. The output of baseline and multithreaded sizes are sanity checks to ensure that the size of both dictionaries are the same, to ensure proper allocation with both techniques. <br />

We can change the number of threads used by changing the input to the *encode_multithreaded* function. In this test we utilize 4 threads: <br />
![image](https://github.com/user-attachments/assets/8d7a2ad4-bd2f-451b-a376-f3fa6cfdaad5) <br />

The baseline is unchanged, where the encoding using multithreading takes longer at 61.1688 seconds or 1 minute and 1 second. It is slower with less threads, but still faster than the baseline. <br />

Changing the number of threads to 2 we can see the following results: <br />
![image](https://github.com/user-attachments/assets/78737ccf-e888-4862-a03e-b33d8a0436ae) <br />

The baseline is largely unchanged again, where the encoding using multithreading takes 95.0103 seconds or 1 minute and 35 seconds. We can see the trend of the time increasing with less threads used however, any amount of threads being used gives us a faster time than regularly allocating the contents of the file to the dictionary. <br />

## Dictionary Querying
With the execution of the [query_test.cpp](https://github.com/marcm24/acs/blob/main/project4/query_test.cpp) file, we can search for different words within the file, as well as any instance of that word appearing within a larger word (substring in a larger string). Running the file we can see the results for searching different words: <br />

![image](https://github.com/user-attachments/assets/30dd9921-c0cc-412f-b0f4-6fb2e041df9e) <br />
In this case we can see that the actual word "bruh" was found, as well as several keys containing the substring "bruh". The baseline time to search is 0.0355083 seconds and the time with x86 SIMD instructions is 0.0591335 seconds. <br />

![image](https://github.com/user-attachments/assets/0db41b4f-5462-4d66-bb04-1e105f600923) <br />
In this case we can see that the word "nisw" was not found but found as a substring within other keys in the dictionary. The baseline time to search is 0.036393 seconds and the time with x86 SIMD instructions is 0.0574099 seconds. <br />

![image](https://github.com/user-attachments/assets/2b21ee02-3b75-4b2c-983c-714fbef26e74) <br />
In this case we can see that the word "hasjdh" was not found at all in the dictionary either as a word or as a substring. The baseline time to search is 0.0336291 seconds and the time with x86 SIMD instructions is 0.0469737 seconds. <br />

We can see that as we have less instances of the string we search for in the dictionary, the baseline time stays relatively unchanged but the time with x86 SIMD instructions decreases. Despite this trend, the x86 SIMD instructions take longer than the baseline. <br />

## Final Analysis and Conclusions
With the tests conducted with these programs we have found that multithreading can benefit the speed at which encoding takes place. In every case we tested regardless of how many threads were used the time to encode the file was faster than the baseline. As we use more threads, the time will decrease further, as expected. In contrast to our expectations the x86 SIMD instructions actually take longer than the baseline. This could be a result of the overhead of setting up SIMD instructions. The [Intel Optimization Reference Manual](https://www.intel.com/content/www/us/en/content-details/671488/intel-64-and-ia-32-architectures-optimization-reference-manual-volume-1.html) provides insights on the use of SIMD instructions, outlining the steps that the CPU needs to go through to use these instructions. Even though we have a large amount of words to work with, the words themselves are not long and as a result the benefits of the SIMD instructions might not show through. <br />
