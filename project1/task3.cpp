#include <iostream>
#include <vector>
#include <chrono> // high res clock

std::vector<int> initializeMemory(size_t dataSize) { // initialize memory
    return std::vector<int>(dataSize, 0);
}

void writeMemory(std::vector<int>& memory, size_t index, int value) { // function to write to memory
    memory[index] = value;
}

int readMemory(const std::vector<int>& memory, size_t index) { // function to read from memory

    return memory[index];
}

void measureLatency(size_t numOperations, size_t dataSize) { // measure latency
    std::vector<int> memory = initializeMemory(dataSize);
    std::vector<size_t> writeTimes;
    std::vector<size_t> readTimes;

    for (size_t i = 0; i < numOperations; ++i) { // measure write latency
        auto start = std::chrono::high_resolution_clock::now();
        writeMemory(memory, i % dataSize, i);  // write operation
        auto end = std::chrono::high_resolution_clock::now();
        writeTimes.push_back((end - start).count());
    }

    for (size_t i = 0; i < numOperations; ++i) { // measure read latency
        auto start = std::chrono::high_resolution_clock::now();
        readMemory(memory, i % dataSize);  // read operation
        auto end = std::chrono::high_resolution_clock::now();
        readTimes.push_back((end - start).count());
    }

    size_t totalWriteTime = 0;
    size_t totalReadTime = 0;

    for (auto time : writeTimes) totalWriteTime += time; // get total write time
    for (auto time : readTimes) totalReadTime += time; // get total read time

    double avgWriteLatency = double(totalWriteTime) / numOperations;
    double avgReadLatency = double(totalReadTime) / numOperations;

    // Calculate throughput
    double writeThroughput = double(numOperations) / (totalWriteTime / 1e9); // ops per second
    double readThroughput = double(numOperations) / (totalReadTime / 1e9); // ops per second

    std::cout << "Average Write Latency: " << avgWriteLatency << " ns\n";
    std::cout << "Average Read Latency: " << avgReadLatency << " ns\n";
    std::cout << "Write Throughput: " << writeThroughput << " ops/s\n";
    std::cout << "Read Throughput: " << readThroughput << " ops/s\n";
}

int main() {
    size_t numOperations = 1000000;  // Number of read/write operations
    size_t dataSize = 1024 * 1024;   // Size of simulated memory (1 MB)

    measureLatency(numOperations, dataSize);

    return 0;
}
