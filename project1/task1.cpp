#include <iostream>
#include <vector>
#include <chrono> // high res clock

void read_latency(int mem_size, int iterations){
    volatile int mem_alloc = mem_size / sizeof(int); // allocation in the memory
    std::vector<int> memory(mem_alloc); // initialize vector

    volatile int tmp = 0; // int for reading

    auto start = std::chrono::high_resolution_clock::now(); // start measuring time
    for (int i = 0; i < iterations; i++) {
        tmp += memory[i % mem_alloc]; // access within the memory
    }
    auto end = std::chrono::high_resolution_clock::now(); // stop measuring time

    auto time = end - start; // time difference

    float access_latency = float(time.count()) / iterations;

    std::cout << "read latency per access: " << access_latency <<  "ns" << std::endl;
}

void write_latency(int mem_size, int iterations){
    volatile int mem_alloc = mem_size / sizeof(int); // allocation in the memory
    std::vector<int> memory(mem_alloc); // initialize vector

    auto start = std::chrono::high_resolution_clock::now(); // start measuring time
    for (int i = 0; i < iterations; i++) {
        memory[i % mem_alloc] = i; // write to memory
    }
    auto end = std::chrono::high_resolution_clock::now(); // stop measuring time

    auto time = end - start; // time difference

    float access_latency = float(time.count()) / iterations;

    std::cout << "write latency per access: " << access_latency <<  "ns" << std::endl;
}

void main_mem_read(int mem_size, int iterations) {
    volatile int mem_alloc = mem_size / sizeof(int); // allocation in the memory
    std::vector<int> memory(mem_alloc); // initialize vector

    volatile int tmp = 0; // int for reading

    auto start = std::chrono::high_resolution_clock::now(); // start measuring time
    for (int g = 0; g < 16; g++) {
        for (int i = 0; i < iterations; i++) {
            tmp += memory[i % mem_alloc]; // access within the memory
        }
    }
    auto end = std::chrono::high_resolution_clock::now(); // stop measuring time

    auto time = end - start; // time difference

    float access_latency = float(time.count()) / (iterations + 16) ; // add 16 for extra iterations

    std::cout << "read latency per access: " << access_latency <<  "ns" << std::endl;
}

void main_mem_write(int mem_size, int iterations) {
    volatile int mem_alloc = mem_size / sizeof(int); // allocation in the memory
    std::vector<int> memory(mem_alloc); // initialize vector

    auto start = std::chrono::high_resolution_clock::now(); // start measuring time
    for (int g = 0; g < 16; g++) {
        for (int i = 0; i < iterations; i++) {
            memory[i % mem_alloc] = i; // write to memory
        }
    }
    auto end = std::chrono::high_resolution_clock::now(); // stop measuring time

    auto time = end - start; // time difference

    float access_latency = float(time.count()) / (iterations + 16); // add 16 for extra iterations

    std::cout << "write latency per access: " << access_latency <<  "ns" << std::endl;
}

int main(){
    int l1_cache_size = 192 * 1024; // in kilobytes
    int l2_cache_size = 3 * 1024 * 1024; // in megabytes
    int l3_cache_size = 8 * 1024 * 1024; // in megabytes
    int main_mem_size = 1 * 1024 * 1024 * 1024; // in gigabytes

    std::cout << "------------------------------" << std::endl;
    std::cout << "L1 Cache (" << l1_cache_size / 1024 << "KB):" << std::endl;
    read_latency(l1_cache_size, 1000);
    write_latency(l1_cache_size, 1000);
    std::cout << "------------------------------" << std::endl;

    std::cout << "L2 Cache (" << l2_cache_size / 1024 / 1024 << "MB):"<< std::endl;
    read_latency(l2_cache_size, 1000);
    write_latency(l2_cache_size, 1000);
    std::cout << "------------------------------" << std::endl;

    std::cout << "L3 Cache (" << l3_cache_size / 1024 / 1024 << "MB):" << std::endl;
    read_latency (l3_cache_size, 1000);
    write_latency(l3_cache_size, 1000);
    std::cout << "------------------------------" << std::endl;
    
    std::cout << "Main Memory (" << main_mem_size / 1024 / 1024 / 1024 << "GB):" << std::endl;\
    main_mem_read(main_mem_size, 1000); // scale up for 16 gigabytes
    main_mem_write(main_mem_size, 1000); // scale up for 16 gigabytes
    std::cout << "------------------------------" << std::endl;
}