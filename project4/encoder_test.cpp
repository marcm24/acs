/*
This file is for the testing of the time it takes to encode the file into the dictionary.
*/

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <chrono> // For high-resolution clock
#include <thread> // For multithreading
#include <immintrin.h> // For x86 SIMD instructions
#include <fstream> // For file input
#include <map> // For key-value pairs
#include <mutex>
#include <vector>

// Thread-safe map insertion
std::mutex global_mutex;

// Baseline encoding function
std::map<std::string, int> encode(const std::string& filename) {
    auto start_time = std::chrono::high_resolution_clock::now(); // Start timing

    std::string word;
    std::map<std::string, int> dictionary;
    std::ifstream column(filename);
    int line = 1;
    if (column.is_open()) {
        while (std::getline(column, word)) {
            dictionary[word] = line;
            line++;
        }
        column.close();
    } else {
        std::cerr << "error: can't open file" << std::endl;
    }

    auto end_time = std::chrono::high_resolution_clock::now(); // End timing
    std::chrono::duration<double> elapsed_time = end_time - start_time;
    std::cout << "baseline encoding completed in " << elapsed_time.count() << " seconds" << std::endl;

    return dictionary;
}

// Process a chunk of the file for multithreaded encoding
void process_lines(const std::vector<std::string>& lines, size_t start, size_t end, std::map<std::string, int>& dictionary, int start_line) {
    for (size_t i = start; i < end; ++i) {
        dictionary[lines[i]] = start_line++;
    }
}

// Multithreaded encoding function
std::map<std::string, int> encode_multithreaded(const std::string& filename, int num_threads) {
    auto start_time = std::chrono::high_resolution_clock::now();

    // Read the entire file into memory
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "error: can't open file" << std::endl;
        return {};
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    file.close();

    // Divide the lines among threads
    size_t total_lines = lines.size();
    size_t chunk_size = (total_lines + num_threads - 1) / num_threads;

    std::vector<std::thread> threads;
    std::vector<std::map<std::string, int>> partial_dictionaries(num_threads);

    for (int i = 0; i < num_threads; ++i) {
        size_t start = i * chunk_size;
        size_t end = std::min(start + chunk_size, total_lines);
        threads.emplace_back(process_lines, std::ref(lines), start, end, std::ref(partial_dictionaries[i]), start + 1);
    }

    // Wait for all threads to complete
    for (auto& thread : threads) {
        thread.join();
    }

    // Merge partial dictionaries
    std::map<std::string, int> final_dictionary;
    for (const auto& partial : partial_dictionaries) {
        std::lock_guard<std::mutex> lock(global_mutex);
        final_dictionary.insert(partial.begin(), partial.end());
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_time = end_time - start_time;
    std::cout << "optimized multithreaded encoding completed in " << elapsed_time.count() << " seconds with " << num_threads << " threads" << std::endl;

    return final_dictionary;
}

void encode_file(std::map<std::string, int> dictionary, std::string output_name) {
    std::ofstream outfile(output_name);

    if(!outfile.is_open()) {
        std::cerr << "error: unable to open file \"" << output_name << "\"" << std::endl;
    }

    for (const auto& pair : dictionary) {
        int value = pair.second; // integer value
        outfile << value << std::endl; // write the value to the file
    }

    outfile.close();
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];

    // Measure performance of baseline
    std::cout << "Running baseline encoding..." << std::endl;
    std::map<std::string, int> dictionary_baseline = encode(filename);
    
    // Measure performance of optimized multithreaded version
    std::cout << "running optimized multithreaded encoding..." << std::endl;
    int num_threads = std::thread::hardware_concurrency(); // Number of available hardware threads
    std::map<std::string, int> dictionary_multithreaded = encode_multithreaded(filename, num_threads);

    encode_file(dictionary_multithreaded, "Column_encoded.txt");

    std::cout << "baseline size: " << dictionary_baseline.size() << std::endl;
    std::cout << "multithreaded size: " << dictionary_multithreaded.size() << std::endl;

    return 0;
}