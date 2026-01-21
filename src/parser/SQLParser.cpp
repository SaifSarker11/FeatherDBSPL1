#include "SQLParser.h"
#include <stdexcept>

SQLParser::SQLParser(Tokenizer &tokenizer) : tokenizer(tokenizer)
{
	advance();
}

void SQLParser::advance()
{
	if (tokenizer.hasNext())
	{
		currentToken = tokenizer.nextToken();
		currentType = tokenizer.getLastTokenType();
	}
	else
	{
		currentToken.clear();
		currentType = Tokenizer::TokenType::END;
	}
}

void SQLParser::expect(const std::string &value)
{
	if (currentToken != value)
	{
		throw std::runtime_error("Expected '" + value + "', got '" + currentToken + "'");
	}
	advance();
}

void SQLParser::expect(Tokenizer::TokenType type)
{
	if (currentType != type)
	{
		throw std::runtime_error("Unexpected token type");
	}
	advance();
}

std::unique_ptr<AST> SQLParser::parse()
{
	if (currentType != Tokenizer::TokenType::KEYWORD)
	{
		throw std::runtime_error("Expected SQL keyword");
	}
	if (currentToken == "SELECT")
		return parseSelect();
	if (currentToken == "INSERT")
		return parseInsert();
	if (currentToken == "UPDATE")
		return parseUpdate();
	if (currentToken == "DELETE")
		return parseDelete();
	throw std::runtime_error("Unknown SQL command");
}

std::unique_ptr<AST> SQLParser::parseSelect()
{
	advance(); // consume SELECT
	auto columns = parseIdentifierList();
	expect("FROM");

	std::string table;
	std::unique_ptr<AST> nestedSource = nullptr;

	if (currentToken == "(")
	{
		advance(); // consume (
		nestedSource = parse();
		expect(")"); // consume )
	}
	else
	{
		table = currentToken;
		expect(Tokenizer::TokenType::IDENTIFIER);
	}

	std::string condition;
	if (currentToken == "WHERE")
	{
		advance();
		while (currentType != Tokenizer::TokenType::END && currentToken != ")")
		{
			condition += currentToken + " ";
			advance();
		}
	}

	return std::make_unique<SelectStatement>(columns, table, condition, std::move(nestedSource));
}

std::unique_ptr<AST> SQLParser::parseInsert()
{
	advance(); // INSERT
	expect("INTO");
	std::string table = currentToken;
	expect(Tokenizer::TokenType::IDENTIFIER);

	// Handle (col1, col2)
	expect("(");
	auto columns = parseIdentifierList();
	expect(")");

	expect("VALUES");

	// Handle (val1, val2)
	expect("(");
	auto values = parseIdentifierList();
	expect(")");

	return std::make_unique<InsertStatement>(table, columns, values);
}

std::unique_ptr<AST> SQLParser::parseUpdate()
{
	advance();
	std::string table = currentToken;
	advance();
	expect("SET");
	std::string column = currentToken;
	advance();
	expect("=");
	std::string value = currentToken;
	advance();
	std::string condition;
	if (currentToken == "WHERE")
	{
		advance();
		condition = currentToken;
		advance();
	}
	return std::make_unique<UpdateStatement>(table, column, value, condition);
}

std::unique_ptr<AST> SQLParser::parseDelete()
{
	advance();
	expect("FROM");
	std::string table = currentToken;
	advance();
	std::string condition;
	if (currentToken == "WHERE")
	{
		advance();
		condition = currentToken;
		advance();
	}
	return std::make_unique<DeleteStatement>(table, condition);
}

std::vector<std::string> SQLParser::parseIdentifierList()
{
	std::vector<std::string> list;

	// Just grab the identifier
	list.push_back(currentToken);
	advance();

	while (currentToken == ",")
	{
		advance(); // consume ,
		list.push_back(currentToken);
		advance();
	}
	return list;
}