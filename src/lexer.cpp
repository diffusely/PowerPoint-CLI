#include "lexer.h"

Lexer::Lexer()
{
 
    m_commands = {"add", "remove", "help", "history", "show", "clear", 
                  "export", "import", "save", "load", "exit", "quit"};
}

std::vector<Token> Lexer::tokenize(const std::string& input)
{
    std::vector<Token> tokens;
    
    if (input.empty())
	{
        return tokens;
    }
    
    size_t pos = 0;
    const size_t length = input.length();
    
    while (pos < length)
	{
        // Skip whitespace
        if (std::isspace(input[pos]))
		{
            skipWhitespace(input, pos);
            continue;
        }
        
        // Handle quoted strings
        if (input[pos] == '"' || input[pos] == '\'')
		{
            std::string quotedStr = extractQuotedString(input, pos);
            if (!quotedStr.empty()) {
                tokens.emplace_back(TokenType::STRING, quotedStr);
            }
            continue;
        }

        std::string word;
        while (pos < length && !std::isspace(input[pos]) && 
               input[pos] != '"' && input[pos] != '\'')
		{
            word += input[pos++];
        }
        
        if (!word.empty())
		{
            TokenType type = determineTokenType(word);
            tokens.emplace_back(type, word);
        }
    }
    
    return tokens;
}

std::vector<Token> Lexer::tokenizer(const std::string& input)
{
    return tokenize(input);
}

bool Lexer::isCommand(const std::string& word) const
{
    std::string lowerWord = word;
    std::transform(lowerWord.begin(), lowerWord.end(), lowerWord.begin(), ::tolower);
    
    return std::find(m_commands.begin(), m_commands.end(), lowerWord) != m_commands.end();
}

bool Lexer::isNumber(const std::string& word) const
{
    if (word.empty()) return false;
    
    size_t start = 0;
    if (word[0] == '-' || word[0] == '+') {
        start = 1;
        if (word.length() == 1) return false;
    }
    
    bool hasDecimal = false;
    for (size_t i = start; i < word.length(); ++i) {
        if (word[i] == '.' && !hasDecimal) {
            hasDecimal = true;
        } else if (!isDigit(word[i])) {
            return false;
        }
    }
    
    return true;
}

bool Lexer::isQuotedString(const std::string& word) const
{
    return word.length() >= 2 && 
           ((word.front() == '"' && word.back() == '"') ||
            (word.front() == '\'' && word.back() == '\''));
}

void Lexer::addCommand(const std::string& command)
{
    std::string lowerCmd = command;
    std::transform(lowerCmd.begin(), lowerCmd.end(), lowerCmd.begin(), ::tolower);
    
    if (std::find(m_commands.begin(), m_commands.end(), lowerCmd) == m_commands.end()) {
        m_commands.push_back(lowerCmd);
    }
}

void Lexer::printTokens(const std::vector<Token>& tokens) const
{
    std::cout << "=== Lexer Tokens ===" << std::endl;
    for (size_t i = 0; i < tokens.size(); ++i) {
        std::cout << "[" << i << "] ";
        tokens[i].print();
    }
    std::cout << "===================" << std::endl;
}

TokenType Lexer::determineTokenType(const std::string& word) const
{
    if (isCommand(word)) {
        return TokenType::COMMAND;
    } else if (isNumber(word)) {
        return TokenType::NUMBER;
    } else if (isQuotedString(word)) {
        return TokenType::STRING;
    } else if (isAlphaNumeric(word[0]) || word[0] == '_') {
        return TokenType::IDENTIFIER;
    } else {
        return TokenType::ARGUMENT;
    }
}

std::string Lexer::extractQuotedString(const std::string& input, size_t& pos) const
{
    if (pos >= input.length()) return "";
    
    char quote = input[pos++]; // Skip opening quote
    std::string result;
    
    while (pos < input.length() && input[pos] != quote)
	{
        if (input[pos] == '\\' && pos + 1 < input.length())
		{
            pos++;
            switch (input[pos]) {
                case 'n': result += '\n'; break;
                case 't': result += '\t'; break;
                case 'r': result += '\r'; break;
                case '\\': result += '\\'; break;
                case '"': result += '"'; break;
                case '\'': result += '\''; break;
                default: result += input[pos]; break;
            }
        }
		else
		{
            result += input[pos];
        }
        pos++;
    }
    
    if (pos < input.length())
	{
        pos++;
    }
    
    return result;
}

void Lexer::skipWhitespace(const std::string& input, size_t& pos) const
{
    while (pos < input.length() && std::isspace(input[pos]))
	{
        pos++;
    }
}

bool Lexer::isAlpha(char c) const
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool Lexer::isDigit(char c) const
{
    return c >= '0' && c <= '9';
}

bool Lexer::isAlphaNumeric(char c) const
{
    return isAlpha(c) || isDigit(c);
}