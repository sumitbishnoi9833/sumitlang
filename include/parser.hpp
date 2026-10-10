#pragma once

#include "lexer.hpp"

#include <memory>
#include <string>
#include <vector>

enum class NodeType {
    Assignment,
    Display,

    Number,
    String,
    Identifier,

    BinaryExpression
};

struct ASTNode {
    NodeType type;
    std::string value;

    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;
};

class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens);

    std::vector<std::unique_ptr<ASTNode>> parse();

private:
    const std::vector<Token>& tokens;
    size_t position = 0;

    const Token& current() const;
    const Token& advance();

    bool check(TokenType type) const;
    bool checkValue(const std::string& value) const;

    void expect(TokenType type);

    std::unique_ptr<ASTNode> parseStatement();

  
    std::unique_ptr<ASTNode> parseExpression();
    std::unique_ptr<ASTNode> parseTerm();
    std::unique_ptr<ASTNode> parseFactor();
    std::unique_ptr<ASTNode> parsePrimary();

    std::unique_ptr<ASTNode> makeNode(
        NodeType type,
        const std::string& value
    );
};