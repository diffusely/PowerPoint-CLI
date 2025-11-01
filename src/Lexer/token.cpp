#include "token.h"

Token::Token() 
	: m_type(TokenType::UNKNOWN)
	, m_value("")
{}

Token::Token(TokenType type, const std::string& value)
	: m_type(type)
	, m_value(value)
{}

TokenType Token::getType() const
{
	return m_type;
}

std::string Token::getValue() const
{
	return m_value;
}

void Token::setType(TokenType type)
{
	m_type = type;
}

void Token::setValue(const std::string& value)
{
	m_value = value;
}

bool Token::isCommand() const
{
	return m_type == TokenType::COMMAND;
}

bool Token::isArgument() const
{
	return m_type == TokenType::ARGUMENT;
}

bool Token::isString() const
{
	return m_type == TokenType::STRING;
}

bool Token::isNumber() const
{
	return m_type == TokenType::NUMBER;
}

void Token::print() const
{
	std::string typeStr;
	switch (m_type)
	{
		case TokenType::COMMAND:     typeStr = "COMMAND";     break;
		case TokenType::ARGUMENT:    typeStr = "ARGUMENT";    break;
		case TokenType::STRING:      typeStr = "STRING";      break;
		case TokenType::NUMBER:      typeStr = "NUMBER";      break;
		case TokenType::IDENTIFIER:  typeStr = "IDENTIFIER";  break;
		case TokenType::WHITESPACE:  typeStr = "WHITESPACE";  break;
		case TokenType::NEWLINE:     typeStr = "NEWLINE";     break;
		case TokenType::UNKNOWN:     typeStr = "UNKNOWN";     break;
		default:                     typeStr = "UNDEFINED";   break;
	}

	std::cout << typeStr << ": \"" << m_value << "\"" << std::endl;
}