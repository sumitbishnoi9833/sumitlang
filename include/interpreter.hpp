#pragma once

#include "parser.hpp"

#include <string>
#include <unordered_map>
#include <vector>

class Interpreter {
public:
    void run(const std::vector<std::unique_ptr<ASTNode>>& program);

private:
    std::unordered_map<std::string, std::string> variables;

    std::string evaluate(const ASTNode* node);
};