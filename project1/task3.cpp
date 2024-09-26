#include <iostream>
#include <vector>
#include <chrono> // high res clock
#include <cstdlib> // random access

std::vector<int> initializeMemory(size_t dataSize) { // initialize memory
    return std::vector<int>(dataSize, 0);
}

void writeMemory(std::vector<int>& memory, size_t index, int value) { // write to memory
    memory[index] = value;
}

int readMemory(const std::vector<int>& memory, size_t index) { // read from memory
    return memory[index];
}

void measureLatency(size_t numOperations, size_t dataSize, bool sequentialAccess) { // measure latency
    std::vector<int> memory = initializeMemory(dataSize);
    std::vector<size_t> writeTimes;
    std::vector<size_t> readTimes;

    // measure write latency
    for (size_t i = 0; i < numOperations; i++) {
        auto start = std::chrono::high_resolution_clock::now();
        
        size_t index;
        if (sequentialAccess) {
            index = i % dataSize; // sequential access
        } else {
            index = rand() % dataSize; // random access
        }
        
        writeMemory(memory, index, i);  // write operation
        auto end = std::chrono::high_resolution_clock::now();
        writeTimes.push_back((end - start).count());
    }

    // measure read latency
    for (size_t i = 0; i < numOperations; i++) {
        auto start = std::chrono::high_resolution_clock::now();
        
        size_t index;
        if (sequentialAccess) {
            index = i % dataSize; // sequential access
        } else {
            index = rand() % dataSize; // random access
        }
        
        readMemory(memory, index);  // read operation
        auto end = std::chrono::high_resolution_clock::now();
        readTimes.push_back((end - start).count());
    }

    // calculate total write and read times
    size_t totalWriteTime = 0;
    size_t totalReadTime = 0;
    for (auto time : writeTimes) totalWriteTime += time; // Get total write time
    for (auto time : readTimes) totalReadTime += time; // Get total read time

    double avgWriteLatency = double(totalWriteTime) / numOperations;
    double avgReadLatency = double(totalReadTime) / numOperations;

    double writeThroughput = double(numOperations) / (totalWriteTime / 1e9); // ops per second
    double readThroughput = double(numOperations) / (totalReadTime / 1e9); // ops per second

    std::cout << "Average Write Latency: " << avgWriteLatency << " ns\n";
    std::cout << "Average Read Latency: " << avgReadLatency << " ns\n";
    std::cout << "Write Throughput: " << writeThroughput << " ops/s\n";
    std::cout << "Read Throughput: " << readThroughput << " ops/s\n";
}

int main() {
    size_t dataSize = 1024 * 1024;   // simulated memory (1 MB)

    std::cout << "Testing with Higher Throughput:" << std::endl;
    measureLatency(1000000, dataSize, true); // sequential access for higher throughput
    std::cout << "---------------------------------------------------------------" << std::endl;

    std::cout << "Testing with Lower Throughput:" << std::endl;
    measureLatency(1000000, dataSize, false); // random access for lower throughput

    return 0;
}
