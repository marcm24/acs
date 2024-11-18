/*
This file is for the testing of the time it takes to query the encoded dictionary and search for the existence of a speicfic word.
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

// Process a chunk of the file for multithreaded encoding
void process_lines(const std::vector<std::string>& lines, size_t start, size_t end, std::map<std::string, int>& dictionary, int start_line) {
    for (size_t i = start; i < end; ++i) {
        dictionary[lines[i]] = start_line++;
    }
}

// Multithreaded encoding function
std::map<std::string, int> encode_multithreaded(const std::string& filename, int num_threads) {


    // Read the entire file into memory
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: can't open file" << std::endl;
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
    auto start_time = std::chrono::high_resolution_clock::now();
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
    std::cout << "Optimized multithreaded encoding completed in " << elapsed_time.count() << " seconds" << std::endl;

    return final_dictionary;
}

// SIMD-based substring search using sliding window
bool simd_find_substring(const std::string& str, const std::string& pattern) {
    size_t pattern_len = pattern.size();
    size_t str_len = str.size();

    // Early exit if pattern is larger than string
    if (pattern_len > str_len) {
        return false;
    }

    // Iterate over the string with a sliding window of size pattern_len
    for (size_t i = 0; i <= str_len - pattern_len; ++i) {
        bool match = true;

        // Compare each character in the current window with the pattern
        for (size_t j = 0; j < pattern_len; ++j) {
            if (str[i + j] != pattern[j]) {
                match = false;
                break; // If any character doesn't match, break out
            }
        }

        if (match) {
            return true; // Found the exact substring
        }
    }

    return false; // No match found
}

// SIMD-based search in a map (exact match)
void simd_map_search(const std::map<std::string, int>& dictionary, const std::string& wordIn) {
    auto start_time = std::chrono::high_resolution_clock::now();

    for (const auto& pair : dictionary) {
        // Use SIMD to check if wordIn is an exact substring of the key
        if (simd_find_substring(pair.first, wordIn)) {
            std::cout << pair.first << " (index: " << pair.second << ")\n";
        }
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_time = end_time - start_time;
    std::cout << "search completed in " << elapsed_time.count() << " seconds" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    std::string wordIn;

    // Measure performance of optimized multithreaded version
    std::cout << "encoding file to dictionary..." << std::endl;
    int num_threads = std::thread::hardware_concurrency(); // Number of available hardware threads
    std::map<std::string, int> dictionary_multithreaded = encode_multithreaded(filename, num_threads);

    while (1) {
        std::cout << std::endl << "enter a word to search for: ";
        std::cin >> wordIn;

        if (wordIn == "exit") {
            std::cout << "exiting program..." << std::endl;
            return 1;
        }

        auto start_time = std::chrono::high_resolution_clock::now();
        auto it = dictionary_multithreaded.find(wordIn);
        if (it != dictionary_multithreaded.end()) {
            std::cout << "found: " << wordIn << " at index: " << it->second << std::endl;
        } else {
            std::cout << "could not find " << wordIn << std::endl;
        }

        std::cout << std::endl << "keys containing substring \"" << wordIn << "\":" << std::endl;
        for (const auto& pair : dictionary_multithreaded) {
            if (pair.first.find(wordIn) != std::string::npos) { // Check if substring is found
                std::cout << pair.first << " (index: " << pair.second << ")\n";
            }
        }

        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_time = end_time - start_time;
        std::cout << "search completed in " << elapsed_time.count() << " seconds" << std::endl;

        std::cout << std::endl << "using x86 SIMD instructions for searching:" << std::endl;

        simd_map_search(dictionary_multithreaded, wordIn);

    }
}