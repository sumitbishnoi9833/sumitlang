#pragma once

#include <string>
#include <vector>

enum class TokenType {
    Identifier,
    Number,
    String,

    Equal,
    Plus,
    Minus,
    Star,
    Slash,

    Greater,
    Less,
    GreaterEqual,
    LessEqual,
    EqualEqual,
    NotEqual,

    LeftParen,
    RightParen,

    LeftBrace,
    RightBrace,

    If,

    Semicolon,

    EndOfFile,
    Invalid
};

struct Token {
    TokenType type;
    std::string value;
    int line;
    int column;
};

class Lexer {
public:
    explicit Lexer(const std::string& source);

    std::vector<Token> tokenize();

private:
    std::string source;
    size_t position = 0;
    int line = 1;
    int column = 1;

    char current() const;
    char peek() const;

    void advance();

    void skipWhitespace();

    Token readNumber();
    Token readIdentifier();
    Token readString();

    Token makeToken(TokenType type, const std::string& value,
                    int startLine, int startColumn);
};   