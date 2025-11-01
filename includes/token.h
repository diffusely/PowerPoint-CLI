#pragma once

#include <iostream>
#include <string>

enum class TokenType {
    COMMAND,
    ARGUMENT,
    STRING,
    NUMBER,
    IDENTIFIER,
    WHITESPACE,
    NEWLINE,
    UNKNOWN
};

class Token
{
public:
    Token();
    Token(TokenType type, const std::string& value);
    
    TokenType getType() const;
    std::string getValue() const;
    void setType(TokenType type);
    void setValue(const std::string& value);
    
    bool isCommand() const;
    bool isArgument() const;
    bool isString() const;
    bool isNumber() const;
    
    void print() const;

private:
    TokenType m_type;
    std::string m_value;
};