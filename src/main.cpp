#include <parsec/Lexer.h>
#include <parsec/Token.h>

#include <iostream>
#include <vector>

using namespace parsec;

int main() {
    Lexer lexer;

    try {
        const char* src = R"(
            // Single line comment
            int main() {
                string message = "Hello, World!\n";
                it x = 42;
                float pi = 3.14;
                
                /* Multi-line comment
                   for testing purposes */
                
                if (x >= 40 && x <= 50) {
                    bool isValid = true;
                    return x != 0 || !isValid;
                } else {
                    return false;
                }
            }
        )";

        std::vector<Token> tokens = lexer.generateTokens(src);

        std::cout << "Tokens (" << tokens.size() << "):\n";
        for (size_t i = 0; i < tokens.size(); ++i) {
            const auto& tk = tokens[i];
            std::cout << i + 1 << ". " << getTokenTypeName(tk.getType());

            if (!tk.getValue().empty()) {
                std::cout << " -> \"" << tk.getValue() << "\"";
            }

            std::cout << "\n";
        }
        std::cout << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Tokenization error: " << e.what() << "\n";
    }

    return 0;
}