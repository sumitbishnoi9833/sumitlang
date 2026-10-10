#include "lexer.hpp"
#include "parser.hpp"
#include "interpreter.hpp"

#include <fstream>
#include <iostream>

int main(int argc, char* argv[]) {

    if (argc != 2) {
        std::cerr << "Usage: sumit <file.sum>\n";
        return 1;
    }

    std::ifstream file(argv[1]);

    if (!file) {
        std::cerr << "Cannot open file: "
                  << argv[1] << '\n';
        return 1;
    }

    std::string source(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>()
    );

    try {

        Lexer lexer(source);
        auto tokens = lexer.tokenize();

        Parser parser(tokens);
        auto program = parser.parse();

        Interpreter interpreter;
        interpreter.run(program);

    } catch (const std::exception& error) {

        std::cerr << "Error: "
                  << error.what()
                  << '\n';

        return 1;
    }

    return 0;
}
