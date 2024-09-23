#include <iostream>
#include <vector>
#include <chrono>

void read_latency(size_t mem_size, int iterations){
    int mem_alloc = mem_size / sizeof(int); // allocation in the memory
    std::vector<int> memory(mem_alloc); // initialize array

    int tmp = 0; // int for reading

    auto start = std::chrono::high_resolution_clock::now(); // start measuring time
    for (int i = 0; i < iterations; i++) {
        tmp += memory[i % mem_alloc]; // access within the memory
    }
    auto end = std::chrono::high_resolution_clock::now(); // stop measuring time

    auto time = end - start; // time difference

    double access_latency = time.count() / iterations;

    std::cout << "read latency per access: " << access_latency <<  "ns" << std::endl;
}

void write_latency(int mem_size, int iterations){
    int mem_alloc = mem_size / sizeof(int); // allocation in the memory
    std::vector<int> memory(mem_alloc); // initialize array

    auto start = std::chrono::high_resolution_clock::now(); // start measuring time
    for (int i = 0; i < iterations; i++) {
        memory[i % mem_alloc] = i; // write to memory
    }
    auto end = std::chrono::high_resolution_clock::now(); // stop measuring time

    auto time = end - start; // time difference

    double access_latency = time.count() / iterations;

    std::cout << "write latency per access: " << access_latency <<  "ns" << std::endl;
}

int main(){
    size_t l1_cache_size = 192 * 1024; // in kilobytes
    size_t l2_cache_size = 3 * 1024 * 1024; // in megabytes
    size_t l3_cache_size = 8 * 1024 * 1024; // in megabytes
    // size_t main_mem_size = 16 * 1024 * 1024 * 1024;

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

    /*
    std::cout << "Main Memory (" << ADD_MAIN_MEMORY_HERE / 1024 / 1024 / 1024 << "GB):" << std::endl;
    read_latency(main_size, 1000);
    write_latency(main_size, 1000);
    std::cout << "------------------------------" << std::endl;
    */

}