#pragma once

#include <parsec/Token.h>

#include <cstdint>
#include <string>
#include <vector>

namespace parsec {

class Lexer {
   private:
    const char* m_src;

    uint32_t m_index;
    uint32_t m_line;
    uint32_t m_offset;

    std::string m_token_buffer;

    std::vector<Token> m_tokens;

   public:
    Lexer();

    std::vector<Token> generateTokens(const char* src);

   private:
    void consumeTokenBuffer();

    bool handleComments();
    void handleLineComments();
    void handleMultilineComments();

    bool handleWhiteSpaces();

    bool handleStringLiterals();

    bool handleAlphaNumericChars();

    bool handleDelimeters();

    bool handleOperators();

    void error();
};

bool isBlank(char chr) noexcept;

bool isAlpha(char chr) noexcept;

bool isDigit(char chr) noexcept;

bool isAlphaNumeric(char chr) noexcept;

bool isOperator(char chr) noexcept;

bool isDelimiter(char chr) noexcept;

}  // namespace parsec