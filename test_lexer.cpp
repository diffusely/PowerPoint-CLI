#include "includes/lexer.h"
#include "includes/token.h"
#include <iostream>

int main() {
    std::cout << "🔍 PowerPoint CLI Lexer Test" << std::endl;
    std::cout << "============================" << std::endl;

    Lexer lexer;
    
    // Test different types of input
    std::vector<std::string> testInputs = {
        "add slide \"My Title\" \"This is content\"",
        "remove 5",
        "help",
        "show presentation",
        "export \"output.html\"",
        "add slide title content 123",
        "history clear"
    };
    
    for (const auto& input : testInputs) {
        std::cout << "\n📝 Input: \"" << input << "\"" << std::endl;
        std::cout << "-----------------------------------" << std::endl;
        
        auto tokens = lexer.tokenize(input);
        
        for (size_t i = 0; i < tokens.size(); ++i) {
            std::cout << "[" << i << "] ";
            tokens[i].print();
        }
    }
    
    std::cout << "\n✅ Lexer test completed successfully!" << std::endl;
    
    return 0;
}
