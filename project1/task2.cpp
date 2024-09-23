#include <iostream>
#include <vector>
#include <chrono>


void memory_test(size_t memory_size, size_t granularity, double read_ration, double write_ratio) {
    std::vector<int> memory(memory_size);
    size_t num_iterations = memory_size / granularity;

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < num_iterations; i++) {

    }
    auto end = std::chrono::high_resolution_clock::now();
}

int main(){
    size_t mem_size = 1024 * 1024 * 1024; // 1GB

    std::vector<size_t> granularities = {64, 256, 1024}; // in bytes
    std::vector<double> ratios = {1.0, 0.0, 0.7, 0.5}; // read-only, write-only, 70:30, 50:50

    for (size_t granularity : granularities) {
        for (double read_ratio : ratios) {
            double write_ratio = 1 - read_ratio;
            memory_test(mem_size, granularity, read_ratio, write_ratio);
        }
    }
}