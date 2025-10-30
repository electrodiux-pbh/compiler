#include <parsec/Lexer.h>

#include <cstdint>
#include <stdexcept>
#include <unordered_map>
#include <vector>

#include "parsec/Token.h"

namespace parsec {

// ---------------------------------
// Mapping Tables
// ---------------------------------

// Keyword mapping for efficient lookup
const std::unordered_map<std::string_view, TokenType> KEYWORDS = {
    {"if", TokenType::IF},     {"else", TokenType::ELSE},     {"for", TokenType::FOR},   {"while", TokenType::WHILE},
    {"do", TokenType::DO},     {"return", TokenType::RETURN}, {"int", TokenType::INT},   {"float", TokenType::FLOAT},
    {"bool", TokenType::BOOL}, {"void", TokenType::VOID},     {"true", TokenType::TRUE}, {"false", TokenType::FALSE},
};

// Operator mapping for efficient lookup (longest operators first for correct matching)
const std::vector<std::pair<std::string_view, TokenType>> OPERATORS = {
    // Two-character operators (check these first)
    {"==", TokenType::EQUAL},
    {"!=", TokenType::NOT_EQUAL},
    {"<=", TokenType::LESS_EQUAL},
    {">=", TokenType::GREATER_EQUAL},
    {"&&", TokenType::LOGICAL_AND},
    {"||", TokenType::LOGICAL_OR},
    // Single-character operators
    {"=", TokenType::ASSIGN},
    {"!", TokenType::LOGICAL_NOT},
    {"<", TokenType::LESS_THAN},
    {">", TokenType::GREATER_THAN},
    {"+", TokenType::PLUS},
    {"-", TokenType::MINUS},
    {"*", TokenType::MULTIPLY},
    {"/", TokenType::DIVIDE},
    {"%", TokenType::MODULO},
    {".", TokenType::DOT},
};

// ---------------------------------
// Lexer Functions
// ---------------------------------

// Parse string literal with escape sequences
Token parseStringLiteral(const char* src, uint32_t& pos, uint32_t& line, uint32_t& offset) {
    std::string value = "\"";  // Start with opening quote
    pos++;                     // Skip opening quote
    offset++;

    while (src[pos] != '\0' && src[pos] != '"') {
        if (src[pos] == '\\' && src[pos + 1] != '\0') {
            // Handle escape sequences
            value += src[pos];  // Add backslash
            pos++;
            offset++;

            switch (src[pos]) {
                case 'n':
                case 't':
                case 'r':
                case '\\':
                case '"':
                case '\'':
                    value += src[pos];
                    break;
                default:
                    value += src[pos];  // Unknown escape, keep as is
            }
        } else if (src[pos] == '\n') {
            line++;
            offset = 1;
            value += src[pos];
        } else {
            value += src[pos];
        }
        pos++;
        offset++;
    }

    if (src[pos] == '"') {
        value += '"';  // Add closing quote
        pos++;         // Skip closing quote
        offset++;
        return Token(TokenType::STRING, value);
    } else {
        // Unclosed string
        return Token(TokenType::INVALID, value);
    }
}

Lexer::Lexer() = default;

constexpr int TOKEN_BUFFER_SIZE = 30;
constexpr char STRING_TERMINATION_CHAR = '\0';
constexpr char NEW_LINE_CHAR = '\n';

std::vector<Token> Lexer::generateTokens(const char* src) {
    m_tokens.clear();

    m_token_buffer.clear();
    m_token_buffer.reserve(TOKEN_BUFFER_SIZE);

    m_index = 0;
    m_line = 1;
    m_offset = 1;

    if (src == nullptr) {
        return m_tokens;
    }

    m_src = src;

    while (m_src[m_index] != STRING_TERMINATION_CHAR) {
        // Handle string literals
        if (handleStringLiterals()) continue;

        // Handle comments
        if (handleComments()) continue;

        // Handle alphanumeric characters and dots in numbers
        if (handleAlphaNumericChars()) continue;

        // Process any buffered identifier/keyword/number
        consumeTokenBuffer();

        // Handle white spaces and newlines
        if (handleWhiteSpaces()) continue;

        // Handle delimeters and semicolons
        if (handleDelimeters()) continue;

        // Handle operators
        if (handleOperators()) continue;

        // Unknown character
        error();
    }

    return m_tokens;
}

// ---------------------------------
// Lexer handlers
// ---------------------------------

bool Lexer::handleComments() {
    char chr0 = m_src[m_index + 0];

    if (chr0 != '/') {
        return false;
    }

    char chr1 = m_src[m_index + 1];
    bool is_comment = false;

    if (chr1 == '/') {
        handleLineComments();
        is_comment = true;
        return true;
    } else if (chr1 == '*') {
        handleMultilineComments();
        is_comment = true;
    }

    return is_comment;
}

void Lexer::handleLineComments() {
    consumeTokenBuffer();

    m_index += 2;
    m_offset += 2;

    while (m_src[m_index] != STRING_TERMINATION_CHAR && m_src[m_index] != NEW_LINE_CHAR) {
        m_index++;
        m_offset++;
    }
}

void Lexer::handleMultilineComments() {
    consumeTokenBuffer();

    m_index += 2;
    m_offset += 2;

    while (m_src[m_index] != STRING_TERMINATION_CHAR) {
        char chr0 = m_src[m_index + 0];
        char chr1 = m_src[m_index + 1];

        if (chr0 == '*' && chr1 == '/') {
            m_index += 2;
            m_offset += 2;
            break;
        } else if (chr0 == NEW_LINE_CHAR) {
            m_index++;
            m_line++;
            m_offset = 1;
        } else {
            m_index++;
            m_offset++;
        }
    }
}

bool Lexer::handleWhiteSpaces() {
    bool contains_whitespace = false;

    while (isBlank(m_src[m_index])) {
        if (!contains_whitespace) {
            consumeTokenBuffer();
        }

        contains_whitespace = true;

        char chr = m_src[m_index];

        if (chr == NEW_LINE_CHAR) {
            m_offset = 1;
            m_line++;
        } else {
            m_offset++;
        }

        m_index++;
    }

    return contains_whitespace;
}

bool Lexer::handleStringLiterals() {
    char chr0 = m_src[m_index];

    if (chr0 == '"') {
        consumeTokenBuffer();

        Token string_token = parseStringLiteral(m_src, m_index, m_line, m_offset);

        if (string_token.getType() == TokenType::INVALID) {
            error();
        }

        m_tokens.push_back(string_token);

        return true;
    }

    return false;
}

bool Lexer::handleAlphaNumericChars() {
    char chr0 = m_src[m_index];

    if (isAlphaNumeric(chr0) || (chr0 == '.' && !m_token_buffer.empty() && isDigit(m_token_buffer.back()))) {
        m_token_buffer.push_back(chr0);
        m_index++;
        m_offset++;

        return true;
    }

    return false;
}

bool Lexer::handleDelimeters() {
    char chr0 = m_src[m_index];

    if (isDelimiter(chr0)) {
        TokenType type;

        switch (chr0) {
            case '(':
                type = TokenType::PARENTHESIS_OPEN;
                break;
            case ')':
                type = TokenType::PARENTHESIS_CLOSE;
                break;
            case '{':
                type = TokenType::BRACE_OPEN;
                break;
            case '}':
                type = TokenType::BRACE_CLOSE;
                break;
            case '[':
                type = TokenType::BRACKET_OPEN;
                break;
            case ']':
                type = TokenType::BRACKET_CLOSE;
                break;
            case ',':
                type = TokenType::COMMA;
                break;
            case ';':
                type = TokenType::SEMICOLON;
                break;
            default:
                type = TokenType::INVALID;
                break;
        }

        if (type == TokenType::INVALID) {
            error();
        }

        m_tokens.push_back(Token(type));
        m_index++;

        return true;
    }

    return false;
}

bool Lexer::handleOperators() {
    // Check operators from longest to shortest to ensure correct matching
    for (const auto& [op_str, token_type] : OPERATORS) {
        size_t op_length = op_str.length();

        // Check if the operator matches at current position
        bool matches = true;
        for (size_t i = 0; i < op_length; ++i) {
            // Check bounds to avoid reading past null terminator
            if (m_src[m_index + i] == STRING_TERMINATION_CHAR || m_src[m_index + i] != op_str[i]) {
                matches = false;
                break;
            }
        }

        if (matches) {
            // Create token with the operator string
            m_tokens.push_back(Token(token_type));

            // Advance position and offset by the operator length
            m_index += op_length;
            m_offset += op_length;

            return true;
        }
    }

    return false;
}

Token bufferToToken(const std::string& buffer) {
    if (buffer.empty()) {
        return Token(TokenType::INVALID, buffer);
    }

    // Check if it's a keyword
    auto keyword_it = KEYWORDS.find(buffer);
    if (keyword_it != KEYWORDS.end()) {
        return Token(keyword_it->second);
    }

    // Check if it's a number
    if (isDigit(buffer[0])) {
        bool is_valid_number = true;
        bool has_decimal_point = false;

        for (char c : buffer) {
            if (c == '.' && !has_decimal_point) {
                has_decimal_point = true;
            } else if (!isDigit(c)) {
                is_valid_number = false;
                break;
            }
        }

        if (is_valid_number) {
            return Token(TokenType::NUMBER, buffer);
        } else {
            return Token(TokenType::INVALID, buffer);
        }
    }

    // Check if it's a valid identifier (starts with letter or underscore)
    if (isAlpha(buffer[0])) {
        bool is_valid_identifier = true;

        for (char c : buffer) {
            if (!isAlphaNumeric(c)) {
                is_valid_identifier = false;
                break;
            }
        }

        if (is_valid_identifier) {
            return Token(TokenType::IDENTIFIER, buffer);
        }
    }

    return Token(TokenType::INVALID, buffer);
}

void Lexer::consumeTokenBuffer() {
    if (!m_token_buffer.empty()) {
        Token token = bufferToToken(m_token_buffer);

        if (token.getType() == TokenType::INVALID) {
            error();
        }

        m_tokens.push_back(token);
        m_token_buffer.clear();
    }
}

void Lexer::error() {
    std::string msg = "Lexer error at line ";
    msg += std::to_string(m_line);
    msg += ":";
    msg += std::to_string(m_offset);

    throw std::runtime_error(msg);
}

// ---------------------------------
// Character Checks
// ---------------------------------

bool isBlank(char chr) noexcept {
    return chr == ' ' || chr == '\n' || chr == '\t' || chr == '\r';
}

bool isAlpha(char chr) noexcept {
    return (chr >= 'A' && chr <= 'Z') || (chr >= 'a' && chr <= 'z') || chr == '_';
}

bool isDigit(char chr) noexcept {
    return chr >= '0' && chr <= '9';
}

bool isAlphaNumeric(char chr) noexcept {
    return isAlpha(chr) || isDigit(chr);
}

bool isOperator(char chr) noexcept {
    return chr == '+' || chr == '-' || chr == '*' || chr == '/' || chr == '%' || chr == '=' || chr == '<' ||
           chr == '>' || chr == '!' || chr == '.' || chr == '&' || chr == '|';
}

bool isDelimiter(char chr) noexcept {
    return chr == '(' || chr == ')' || chr == '{' || chr == '}' || chr == '[' || chr == ']' || chr == ';' || chr == ',';
}

}  // namespace parsec