#include <iostream>
#include <chrono>
#include <vector>
#include <cstdlib>

void multiply(std::vector<int> nums) {
    volatile long result = 0; // prevent compiler optimization
    for (size_t i = 0; i < nums.size(); i++) {
        if (i + 1 >= nums.size()) {
            break;
        } else {
            result += nums[i] * nums[i + 1]; // multiplication operation
        }
    }
}

float runTime(std::vector<int>& array) {
    auto start = std::chrono::high_resolution_clock::now();
    multiply(array);
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = end - start;
    return float(elapsed.count());
}

void cacheTest(size_t array_size) {
    // larger array to simulate TLB misses
    size_t larger_array_size = array_size * 10; 
    std::vector<int> larger_array(larger_array_size);
    
    for (size_t i = 0; i < larger_array_size; i++) { 
        larger_array[i] = rand() % 100; // random value from 0 to 99
    }

    // Access elements in a non-sequential manner to simulate TLB misses
    std::vector<int> access_order(array_size);
    for (size_t i = 0; i < array_size; i++) {
        access_order[i] = rand() % larger_array_size; // random index
    }

    // Create a new vector to hold the accessed elements
    std::vector<int> accessed_elements(array_size);
    for (size_t i = 0; i < array_size; i++) {
        accessed_elements[i] = larger_array[access_order[i]];
    }

    volatile float execution = runTime(accessed_elements);
    std::cout << "Array Size: " << array_size << " | Run Time: " << execution / 1024 << " ns" << std::endl;
}

int main() {
    size_t iterations = 20; // set to run amount of iterations

    for (size_t i = 1; i <= iterations; i++) {
        size_t mem_size = i * 1024; // 1KB to 20KB
        cacheTest(mem_size);
    }

    return 0;
}
