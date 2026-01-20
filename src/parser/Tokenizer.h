#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <vector>
#include <cctype>
#include <stdexcept>

class Tokenizer
{
public:
	enum class TokenType
	{
		KEYWORD,
		IDENTIFIER,
		NUMBER,
		STRING,
		OPERATOR,
		PUNCTUATION,
		END,
		INVALID
	};

	// Constructor
	Tokenizer(const std::string &input);
	bool isPunctuation(char c) const;
	// Get the next token from the input
	std::string nextToken();

	// Check if there are more tokens
	bool hasNext() const;

	// Get the type of the last token
	TokenType getLastTokenType() const;

private:
	std::string input;
	size_t position;
	std::string currentToken;
	TokenType currentTokenType;

	// Helper functions
	bool isWhitespace(char c) const;
	bool isKeyword(const std::string &str) const;
	bool isOperator(char c) const;
	bool isNumber(char c) const;
	bool isIdentifierStart(char c) const;
	bool isIdentifierPart(char c) const;
	bool isStringDelimiter(char c) const;
};

#endif // TOKENIZER_H
