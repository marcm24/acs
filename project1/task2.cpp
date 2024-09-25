#include <iostream>
#include <vector>
#include <chrono> // high res clock
#include <cstring> // memset

// bandwidth: bytes transferred / time
// read ratio goes up, bandwidth goes down

const volatile size_t buffer = 1 * 1024 * 1024 * 1024; // 1GB Buffer

void memory_test(size_t buffer, size_t block_size, size_t num_iterations, float read_ratio) {
    float write_ratio = 1 - read_ratio;
    std::vector<int> memory(buffer); // 1GB Buffer

    size_t bytes_read = 0;
    size_t bytes_written = 0;


    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < num_iterations; i++) {
        if (read_ratio > 0) {
            for (size_t r = 0; r < buffer; r += block_size) { // increment in different access granularities (every 64B, 256B, 1024B)
                volatile int temp = memory[r]; // prevent compiler optimization
                bytes_read += block_size;
            }
            
        }

        if (write_ratio > 0) {
            for (size_t w = 0; w < buffer; w += block_size) {
                volatile int temp = 1;
                memory[w] = temp;
                bytes_written += block_size;
            }
        }

    }

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = end - start;
    size_t total_bytes = bytes_read + bytes_written;
    float bandwidth = float(total_bytes) / elapsed.count();
    std::cout << "Block Size: " << block_size << " B | "
              << "Read Ratio: " << read_ratio * 100 << "% | "
              << "Bandwidth: " << bandwidth << " GB/s" << std::endl;
}

int main(){

    std::vector<size_t> granularities = {64, 256, 1024}; // in bytes
    std::vector<float> ratios = {0.0, 0.5, 0.7, 1.0}; // write-only, 70:30, 50:50, read-only

    for (size_t granularity : granularities) { // run for each granularity
        for (float read_ratio : ratios) { // going through ratios in order
            memory_test(buffer, granularity, 5, read_ratio);
        }
        std::cout << "---------------------------------------------------------------" << std::endl;
    }
}