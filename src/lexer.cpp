#include "lexer.hpp"

#include <cctype>

Lexer::Lexer(const std::string& source)
    : source(source) {}

char Lexer::current() const {
    if (position >= source.size())
        return '\0';

    return source[position];
}

char Lexer::peek() const {
    if (position + 1 >= source.size())
        return '\0';

    return source[position + 1];
}

void Lexer::advance() {
    if (current() == '\n') {
        line++;
        column = 1;
    } else {
        column++;
    }

    position++;
}

Token Lexer::makeToken(
    TokenType type,
    const std::string& value,
    int startLine,
    int startColumn
) {
    return {type, value, startLine, startColumn};
}

void Lexer::skipWhitespace() {
    while (std::isspace(static_cast<unsigned char>(current()))) {
        advance();
    }
}

Token Lexer::readNumber() {
    int startLine = line;
    int startColumn = column;

    std::string value;

    while (std::isdigit(static_cast<unsigned char>(current()))) {
        value += current();
        advance();
    }

    return makeToken(
        TokenType::Number,
        value,
        startLine,
        startColumn
    );
}

Token Lexer::readIdentifier() {
    int startLine = line;
    int startColumn = column;

    std::string value;

    while (
        std::isalnum(static_cast<unsigned char>(current())) ||
        current() == '_'
    ) {
        value += current();
        advance();
    }

    TokenType type = value == "if"
        ? TokenType::If
        : TokenType::Identifier;

    return makeToken(
        type,
        value,
        startLine,
        startColumn
    );
}

Token Lexer::readString() {
    int startLine = line;
    int startColumn = column;

    // Skip opening "
    advance();

    std::string value;

    while (current() != '"' && current() != '\0') {
        value += current();
        advance();
    }

    if (current() == '"') {
        advance();
        return makeToken(
            TokenType::String,
            value,
            startLine,
            startColumn
        );
    }

    return makeToken(
        TokenType::Invalid,
        "Unterminated string",
        startLine,
        startColumn
    );
}

std::vector<Token> Lexer::tokenize() {

    std::vector<Token> tokens;

    while (current() != '\0') {

        skipWhitespace();

        if (current() == '\0')
            break;

        int startLine = line;
        int startColumn = column;

        char c = current();

        if (std::isdigit(static_cast<unsigned char>(c))) {
            tokens.push_back(readNumber());
            continue;
        }

        if (
            std::isalpha(static_cast<unsigned char>(c)) ||
            c == '_'
        ) {
            tokens.push_back(readIdentifier());
            continue;
        }

        if (c == '"') {
            tokens.push_back(readString());
            continue;
        }

        switch (c) {

            case '>':
                if (peek() == '=') {
                    advance();
                    tokens.push_back(
                        makeToken(
                            TokenType::GreaterEqual,
                            ">=",
                            startLine,
                            startColumn
                        )
                    );
                } else {
                    tokens.push_back(
                        makeToken(
                            TokenType::Greater,
                            ">",
                            startLine,
                            startColumn
                        )
                    );
                }
                advance();
                break;

            case '<':
                if (peek() == '=') {
                    advance();
                    tokens.push_back(
                        makeToken(
                            TokenType::LessEqual,
                            "<=",
                            startLine,
                            startColumn
                        )
                    );
                } else {
                    tokens.push_back(
                        makeToken(
                            TokenType::Less,
                            "<",
                            startLine,
                            startColumn
                        )
                    );
                }
                advance();
                break;

            case '=':
                if (peek() == '=') {
                    advance();
                    tokens.push_back(
                        makeToken(
                            TokenType::EqualEqual,
                            "==",
                            startLine,
                            startColumn
                        )
                    );
                } else {
                    tokens.push_back(
                        makeToken(
                            TokenType::Equal,
                            "=",
                            startLine,
                            startColumn
                        )
                    );
                }
                advance();
                break;

            case '!':
                if (peek() == '=') {
                    advance();
                    tokens.push_back(
                        makeToken(
                            TokenType::NotEqual,
                            "!=",
                            startLine,
                            startColumn
                        )
                    );
                } else {
                    tokens.push_back(
                        makeToken(
                            TokenType::Invalid,
                            "!",
                            startLine,
                            startColumn
                        )
                    );
                }
                advance();
                break;

            case '+':
                tokens.push_back(
                    makeToken(TokenType::Plus, "+", startLine, startColumn)
                );
                advance();
                break;

            case '-':
                tokens.push_back(
                    makeToken(TokenType::Minus, "-", startLine, startColumn)
                );
                advance();
                break;

            case '*':
                tokens.push_back(
                    makeToken(TokenType::Star, "*", startLine, startColumn)
                );
                advance();
                break;

            case '/':
                tokens.push_back(
                    makeToken(TokenType::Slash, "/", startLine, startColumn)
                );
                advance();
                break;

            case ';':
                tokens.push_back(
                    makeToken(
                        TokenType::Semicolon,
                        ";",
                        startLine,
                        startColumn
                    )
                );
                advance();
                break;
            case '(':
                tokens.push_back({TokenType::LeftParen, "(", line, column});
                advance();
                break;

            case ')':
                tokens.push_back({TokenType::RightParen, ")", line, column});
                advance();
                break;

            case '{':
                tokens.push_back({TokenType::LeftBrace, "{", line, column});
                advance();
                break;

            case '}':
                tokens.push_back({TokenType::RightBrace, "}", line, column});
                advance();
                break;

            default:
                tokens.push_back(
                    makeToken(
                        TokenType::Invalid,
                        std::string(1, c),
                        startLine,
                        startColumn
                    )
                );
                advance();
                break;
        }
    }

    tokens.push_back(
        makeToken(TokenType::EndOfFile, "", line, column)
    );

    return tokens;
}