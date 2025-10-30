#include <parsec/Token.h>

namespace parsec {

const char* getTokenTypeName(TokenType type) noexcept {
    switch (type) {
        case TokenType::INVALID:
            return "INVALID";
        case TokenType::EOF_TOKEN:
            return "EOF";
        case TokenType::IF:
            return "IF";
        case TokenType::ELSE:
            return "ELSE";
        case TokenType::FOR:
            return "FOR";
        case TokenType::WHILE:
            return "WHILE";
        case TokenType::DO:
            return "DO";
        case TokenType::RETURN:
            return "RETURN";
        case TokenType::INT:
            return "INT";
        case TokenType::FLOAT:
            return "FLOAT";
        case TokenType::BOOL:
            return "BOOL";
        case TokenType::VOID:
            return "VOID";
        case TokenType::TRUE:
            return "TRUE";
        case TokenType::FALSE:
            return "FALSE";
        case TokenType::IDENTIFIER:
            return "IDENTIFIER";
        case TokenType::NUMBER:
            return "NUMBER";
        case TokenType::STRING:
            return "STRING";
        case TokenType::CHAR:
            return "CHAR";
        case TokenType::PLUS:
            return "PLUS";
        case TokenType::MINUS:
            return "MINUS";
        case TokenType::MULTIPLY:
            return "MULTIPLY";
        case TokenType::DIVIDE:
            return "DIVIDE";
        case TokenType::MODULO:
            return "MODULO";
        case TokenType::ASSIGN:
            return "ASSIGN";
        case TokenType::EQUAL:
            return "EQUAL";
        case TokenType::NOT_EQUAL:
            return "NOT_EQUAL";
        case TokenType::LESS_THAN:
            return "LESS_THAN";
        case TokenType::GREATER_THAN:
            return "GREATER_THAN";
        case TokenType::LESS_EQUAL:
            return "LESS_EQUAL";
        case TokenType::GREATER_EQUAL:
            return "GREATER_EQUAL";
        case TokenType::LOGICAL_AND:
            return "LOGICAL_AND";
        case TokenType::LOGICAL_OR:
            return "LOGICAL_OR";
        case TokenType::LOGICAL_NOT:
            return "LOGICAL_NOT";
        case TokenType::PARENTHESIS_OPEN:
            return "PARENTHESIS_OPEN";
        case TokenType::PARENTHESIS_CLOSE:
            return "PARENTHESIS_CLOSE";
        case TokenType::BRACE_OPEN:
            return "BRACE_OPEN";
        case TokenType::BRACE_CLOSE:
            return "BRACE_CLOSE";
        case TokenType::BRACKET_OPEN:
            return "BRACKET_OPEN";
        case TokenType::BRACKET_CLOSE:
            return "BRACKET_CLOSE";
        case TokenType::SEMICOLON:
            return "SEMICOLON";
        case TokenType::COMMA:
            return "COMMA";
        case TokenType::DOT:
            return "DOT";
        default:
            return nullptr;
    }
}

}  // namespace parsec
