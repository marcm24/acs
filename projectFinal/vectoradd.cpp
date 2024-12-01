#include <iostream>
#include <vector>
#include <chrono>
#include <cstdlib> // for rand()

int main() {
    const int arraySize = 1000; // change this to set the size of the vectors
    std::vector<int> a(arraySize);
    std::vector<int> b(arraySize);
    std::vector<int> c(arraySize, 0); // result vector initialized to 0

    // fill vectors with random values
    for (int i = 0; i < arraySize; ++i) {
        a[i] = rand() % 100; // Random values between 0 and 99
        b[i] = rand() % 100; // Random values between 0 and 99
    }

    // start timing
    auto start = std::chrono::high_resolution_clock::now();

    // perform vector addition
    for (int i = 0; i < arraySize; ++i) {
        c[i] = a[i] + b[i];
    }

    // end timing
    auto stop = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = stop - start;

    // output the result for small-sized vectors (optional)
    if (arraySize <= 10) { // limit output to small sizes for readability
        std::cout << "{";
        for (size_t i = 0; i < a.size(); ++i) {
            std::cout << a[i] << (i < a.size() - 1 ? "," : "");
        }
        std::cout << "} + {";
        for (size_t i = 0; i < b.size(); ++i) {
            std::cout << b[i] << (i < b.size() - 1 ? "," : "");
        }
        std::cout << "} = {";
        for (size_t i = 0; i < c.size(); ++i) {
            std::cout << c[i] << (i < c.size() - 1 ? "," : "");
        }
        std::cout << "}" << std::endl;
    } else {
        std::cout << "Vector addition completed for size: " << arraySize << std::endl;
    }

    // output the time taken
    std::cout << "Time taken for addition: " << duration.count() << " ms" << std::endl;

    return 0;
}
