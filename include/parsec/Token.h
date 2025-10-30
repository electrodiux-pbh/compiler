#pragma once

#include <string>

namespace parsec {

enum class TokenType {
    INVALID,
    EOF_TOKEN,

    // Keywords
    IF,
    ELSE,
    FOR,
    WHILE,
    DO,
    RETURN,
    INT,
    FLOAT,
    BOOL,
    VOID,
    TRUE,
    FALSE,

    // Identifiers and Literals
    IDENTIFIER,
    NUMBER,
    STRING,
    CHAR,

    // Operators
    PLUS,           // +
    MINUS,          // -
    MULTIPLY,       // *
    DIVIDE,         // /
    MODULO,         // %
    ASSIGN,         // =
    EQUAL,          // ==
    NOT_EQUAL,      // !=
    LESS_THAN,      // <
    GREATER_THAN,   // >
    LESS_EQUAL,     // <=
    GREATER_EQUAL,  // >=
    LOGICAL_AND,    // &&
    LOGICAL_OR,     // ||
    LOGICAL_NOT,    // !

    // Delimiters
    PARENTHESIS_OPEN,   // (
    PARENTHESIS_CLOSE,  // )
    BRACE_OPEN,         // {
    BRACE_CLOSE,        // }
    BRACKET_OPEN,       // [
    BRACKET_CLOSE,      // ]
    SEMICOLON,          // ;
    COMMA,              // ,
    DOT,                // .
};

const char* getTokenTypeName(TokenType type) noexcept;

class Token {
   private:
    TokenType m_type;
    std::string m_value;  // Store the actual token text

   public:
    explicit Token(TokenType type = TokenType::INVALID, const std::string& value = "") : m_type(type), m_value(value) {}

    TokenType getType() const {
        return m_type;
    }
    void setType(TokenType type) {
        m_type = type;
    }

    const std::string& getValue() const {
        return m_value;
    }
    void setValue(const std::string& value) {
        m_value = value;
    }
};

}  // namespace parsec