#include <iostream>
#include <chrono>
#include <vector>
#include <cstdlib>

void multiply(std::vector<int> nums) {
    volatile long result; // prevent compiler optimization
    for (size_t i = 0; i < nums.size(); i++) {
        if (i + 1 > nums.size()) {
            break;
        } else {
            result += nums[i] * nums[i + 1]; // mulitplication operation
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
    std::vector<int> array(array_size);

    for (size_t i = 0; i < array_size; i++) { 
        array[i] = rand() % 100; // random value from 0 to 99
    }

    volatile float execution = runTime(array);
    std::cout << "Array Size: " << array_size << " | Run Time: " << execution / 1024 << " ns" << std::endl;
}

int main() {
    size_t iterations = 20; // set to run amount of iterations

    for (size_t i = 0; i < iterations; i++) {
        size_t mem_size = i * 1024; // 1KB to 20KB
        cacheTest(mem_size);
    }
}