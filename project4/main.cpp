#include <iostream>
#include <cstdlib>
#include <ctime>  
#include <string>
#include <chrono> // high res clock
#include <thread> // multithreading
#include <immintrin.h> // for x86 SIMD instructions
#include <fstream> // for file input
#include <map> // key value pairs

// multithreaded implementation???
std::map<std::string, int> getWords(std::string filename) {
    std::string word;
    std::map<std::string, int> dictionary;
    std::ifstream column(filename);
    int line = 1;
    if(column.is_open()) {
        while(std::getline(column, word)) {
            dictionary[word] = line;
            line ++;
        }
        column.close();

        return dictionary;

    } else {
        std::cerr << "error: can't open file" << std::endl;
    }

}

int main(int argc, char* argv[]) {
    std::map<std::string, int> dictionary = getWords("Column.txt");
}