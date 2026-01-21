#include "Tokenizer.h"
#include <sstream>
#include <cctype>
#include <stdexcept>

Tokenizer::Tokenizer(const std::string &input)
	: input(input), position(0), currentTokenType(TokenType::INVALID) {}

bool Tokenizer::isWhitespace(char c) const
{
	return std::isspace(c);
}

bool Tokenizer::isKeyword(const std::string &str) const
{
	// extensible
	static const std::vector<std::string> keywords = {
		"SELECT", "INSERT", "UPDATE", "DELETE", "FROM", "WHERE", "AND", "OR", "VALUES", "LIMIT"};
	for (const auto &keyword : keywords)
	{
		if (str == keyword)
		{
			return true;
		}
	}
	return false;
}

bool Tokenizer::isOperator(char c) const
{
	return (c == '=' || c == '>' || c == '<' || c == '!' || c == '+' || c == '-' || c == '*' || c == '/' || c == '%');
}

bool Tokenizer::isNumber(char c) const
{
	return std::isdigit(c);
}

bool Tokenizer::isIdentifierStart(char c) const
{
	return std::isalpha(c) || c == '_';
}

bool Tokenizer::isIdentifierPart(char c) const
{
	return std::isalnum(c) || c == '_';
}

bool Tokenizer::isStringDelimiter(char c) const
{
	return c == '\'';
}
bool Tokenizer::isPunctuation(char c) const
{
	return c == ',' || c == '(' || c == ')' || c == ';';
}

Tokenizer::TokenType Tokenizer::getLastTokenType() const
{
	return currentTokenType;
}

std::string Tokenizer::nextToken()
{
	currentToken.clear();
	currentTokenType = TokenType::INVALID;

	// Skip whitespace
	while (position < input.size() && isWhitespace(input[position]))
	{
		position++;
	}

	if (position >= input.size())
	{
		currentTokenType = TokenType::END;
		return "";
	}

	char currentChar = input[position];

	// Handle string literals
	if (isStringDelimiter(currentChar))
	{
		position++; // Skip the opening quote
		while (position < input.size() && input[position] != '\'')
		{
			currentToken += input[position++];
		}
		if (position < input.size() && input[position] == '\'')
		{
			position++; // Skip the closing quote
			currentTokenType = TokenType::STRING;
		}
		else
		{
			throw std::invalid_argument("Unterminated string literal");
		}
	}
	// Handle numbers
	else if (isNumber(currentChar))
	{
		while (position < input.size() && isNumber(input[position]))
		{
			currentToken += input[position++];
		}
		currentTokenType = TokenType::NUMBER;
	}
	// Handle identifiers 
	else if (isIdentifierStart(currentChar))
	{
		while (position < input.size() && isIdentifierPart(input[position]))
		{
			currentToken += input[position++];
		}
		// Check for keyword
		if (isKeyword(currentToken))
		{
			currentTokenType = TokenType::KEYWORD;
		}
		else
		{
			currentTokenType = TokenType::IDENTIFIER;
		}
	}
	// Handle operators
	else if (isOperator(currentChar))
	{
		currentToken += currentChar;
		position++;
		currentTokenType = TokenType::OPERATOR;
	}
	else if (isPunctuation(currentChar))
	{
		currentToken += currentChar;
		position++;
		currentTokenType = TokenType::PUNCTUATION;
	}

	else
	{
		currentTokenType = TokenType::INVALID;
		throw std::invalid_argument("Invalid character encountered");
	}

	return currentToken;
}

bool Tokenizer::hasNext() const
{
	return position < input.size();
}
