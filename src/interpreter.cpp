#include "interpreter.hpp"

#include <iostream>
#include <stdexcept>

std::string Interpreter::evaluate(const ASTNode* node) {

    switch (node->type) {

        case NodeType::Number:
            return node->value;

        case NodeType::String:
            return node->value;

        case NodeType::Identifier: {

            auto it = variables.find(node->value);

            if (it == variables.end()) {
                throw std::runtime_error(
                    "Undefined variable: " + node->value
                );
            }

            return it->second;
        }

        case NodeType::BinaryExpression: {

            int left = std::stoi(evaluate(node->left.get()));
            int right = std::stoi(evaluate(node->right.get()));

            if (node->value == ">")
                return left > right ? "true" : "false";

            if (node->value == "<")
                return left < right ? "true" : "false";

            if (node->value == ">=")
                return left >= right ? "true" : "false";

            if (node->value == "<=")
                return left <= right ? "true" : "false";

            if (node->value == "==")
                return left == right ? "true" : "false";

            if (node->value == "!=")
                return left != right ? "true" : "false";

            if (node->value == "+")
                return std::to_string(left + right);

            if (node->value == "-")
                return std::to_string(left - right);

            if (node->value == "*")
                return std::to_string(left * right);

            if (node->value == "/") {

                if (right == 0)
                    throw std::runtime_error("Division by zero");

                return std::to_string(left / right);
            }

            throw std::runtime_error(
                "Unknown operator: " + node->value
            );
        }

        default:
            throw std::runtime_error("Invalid expression");
    }
}

void Interpreter::run(
    const std::vector<std::unique_ptr<ASTNode>>& program
) {

    for (const auto& node : program) {

        if (node->type == NodeType::Assignment) {

            variables[node->value] =
                evaluate(node->left.get());
        }

        else if (node->type == NodeType::Display) {

            std::cout
                << evaluate(node->left.get())
                << '\n';
        }
    }
}