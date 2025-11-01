#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>

#include "token.h"


class Lexer
{
public:
    Lexer();
    
    std::vector<Token> tokenize(const std::string& input);
    std::vector<Token> tokenizer(const std::string& input);
    
    bool isCommand(const std::string& word) const;
    bool isNumber(const std::string& word) const;
    bool isQuotedString(const std::string& word) const;
    
    void addCommand(const std::string& command);
    void printTokens(const std::vector<Token>& tokens) const;

private:
    std::vector<std::string> m_commands;
    

    TokenType determineTokenType(const std::string& word) const;
    std::string extractQuotedString(const std::string& input, size_t& pos) const;
    void skipWhitespace(const std::string& input, size_t& pos) const;
    bool isAlpha(char c) const;
    bool isDigit(char c) const;
    bool isAlphaNumeric(char c) const;
};