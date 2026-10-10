#include "parser.hpp"

#include <stdexcept>

Parser::Parser(const std::vector<Token>& tokens)
    : tokens(tokens) {}

const Token& Parser::current() const {
    return tokens[position];
}

const Token& Parser::advance() {
    return tokens[position++];
}

bool Parser::check(TokenType type) const {
    return current().type == type;
}

bool Parser::checkValue(const std::string& value) const {
    return current().value == value;
}

void Parser::expect(TokenType type) {

    if (!check(type)) {

        throw std::runtime_error(
            "Unexpected token [" +
            current().value +
            "] at " +
            std::to_string(current().line) +
            ":" +
            std::to_string(current().column)
        );
    }

    advance();
}

std::unique_ptr<ASTNode> Parser::makeNode(
    NodeType type,
    const std::string& value
) {
    auto node = std::make_unique<ASTNode>();

    node->type = type;
    node->value = value;

    return node;
}

std::vector<std::unique_ptr<ASTNode>> Parser::parse() {

    std::vector<std::unique_ptr<ASTNode>> program;

    while (!check(TokenType::EndOfFile)) {

        program.push_back(parseStatement());
    }

    return program;
}

std::unique_ptr<ASTNode> Parser::parseStatement() {

    // display = expression;
    if (checkValue("display")) {

        advance();

        expect(TokenType::Equal);

        auto node = makeNode(NodeType::Display, "display");

        node->left = parseExpression();

        expect(TokenType::Semicolon);

        return node;
    }

    // identifier = expression;
    if (check(TokenType::Identifier)) {

        std::string name = advance().value;

        expect(TokenType::Equal);

        auto node = makeNode(NodeType::Assignment, name);

        node->left = parseExpression();

        expect(TokenType::Semicolon);

        return node;
    }

    throw std::runtime_error(
        "Unexpected token [" +
        current().value +
        "]"
    );
}

std::unique_ptr<ASTNode> Parser::parseExpression() {

    auto left = parseTerm();

    while (
        check(TokenType::Plus) ||
        check(TokenType::Minus) ||
        check(TokenType::Greater) ||
        check(TokenType::Less) ||
        check(TokenType::GreaterEqual) ||
        check(TokenType::LessEqual) ||
        check(TokenType::EqualEqual) ||
        check(TokenType::NotEqual)
    ) {

        std::string operation = advance().value;

        auto right = parseTerm();

        auto node = makeNode(
            NodeType::BinaryExpression,
            operation
        );

        node->left = std::move(left);
        node->right = std::move(right);

        left = std::move(node);
    }

    return left;
}

std::unique_ptr<ASTNode> Parser::parseTerm() {

    auto left = parseFactor();

    while (
        check(TokenType::Star) ||
        check(TokenType::Slash)
    ) {

        std::string operation = advance().value;

        auto right = parseFactor();

        auto node = makeNode(
            NodeType::BinaryExpression,
            operation
        );

        node->left = std::move(left);
        node->right = std::move(right);

        left = std::move(node);
    }

    return left;
}

std::unique_ptr<ASTNode> Parser::parseFactor() {
    return parsePrimary();
}

std::unique_ptr<ASTNode> Parser::parsePrimary() {
    if (check(TokenType::LeftParen)) {
    advance();

    auto expression = parseExpression();

    expect(TokenType::RightParen);

    return expression;
    }

    if (check(TokenType::Number)) {

        return makeNode(
            NodeType::Number,
            advance().value
        );
    }

    if (check(TokenType::String)) {

        return makeNode(
            NodeType::String,
            advance().value
        );
    }

    if (check(TokenType::Identifier)) {

        return makeNode(
            NodeType::Identifier,
            advance().value
        );
    }

    throw std::runtime_error(
        "Expected a value, got [" +
        current().value +
        "]"
    );
}