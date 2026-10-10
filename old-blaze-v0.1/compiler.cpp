#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char* argv[]) {

    if (argc != 2) {
        std::cout << "Usage: sumit <file.sum>\n";
        return 1;
    }

    std::ifstream file(argv[1]);

    if (!file) {
        std::cout << "Error: Cannot open file.\n";
        return 1;
    }

    std::string line;

    while (std::getline(file, line)) {

        // Check that the statement starts correctly
        if (line.rfind("display = ", 0) != 0) {
            std::cout << "Error: Invalid statement.\n";
            return 1;
        }

        // Remove "display = "
        std::string value = line.substr(10);

        // Check semicolon
        if (value.empty() || value.back() != ';') {
            std::cout << "Error: Missing ';'.\n";
            return 1;
        }

        // Remove semicolon
        value.pop_back();

        // Check for string
        if (value.size() >= 2 &&
            value.front() == '"' &&
            value.back() == '"') {

            value = value.substr(1, value.size() - 2);

            std::cout << value << '\n';
        }

        // Check for number
        else {
            try {
                std::stoi(value);
                std::cout << value << '\n';
            }
            catch (...) {
                std::cout << "Error: Invalid value.\n";
                return 1;
            }
        }
    }

    return 0;
}