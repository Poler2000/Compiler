#include <iostream>

int compile(const char* input, const char* output);

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Usage: ./kompilator $source_file $output_file\n";
    }
    return compile(argv[1], argv[2]);
}