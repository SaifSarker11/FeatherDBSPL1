#include <iostream>
#include "./parser/Tokenizer.h"

int main()
{
	std::string query = "SELECT name, age FROM users WHERE age > 30";
	Tokenizer tokenizer(query);

	while (tokenizer.hasNext())
	{
		std::string token = tokenizer.nextToken();
		Tokenizer::TokenType type = tokenizer.getLastTokenType();

		std::cout << "Token: " << token << ", Type: ";
		switch (type)
		{
		case Tokenizer::TokenType::KEYWORD:
			std::cout << "Keyword";
			break;
		case Tokenizer::TokenType::IDENTIFIER:
			std::cout << "Identifier";
			break;
		case Tokenizer::TokenType::NUMBER:
			std::cout << "Number";
			break;
		case Tokenizer::TokenType::STRING:
			std::cout << "String";
			break;
		case Tokenizer::TokenType::OPERATOR:
			std::cout << "Operator";
			break;
		case Tokenizer::TokenType::END:
			std::cout << "End of Input";
			break;
		default:
			std::cout << "Invalid Token";
		}
		std::cout << std::endl;
	}

	return 0;
}
