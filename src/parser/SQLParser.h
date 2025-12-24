#ifndef SPL_SQLPARSER_H
#define SPL_SQLPARSER_H

#include <string>
#include "Ast.h"

namespace spl {

class SqlParser {
public:
	SqlParser() = default;
	virtual ~SqlParser() = default;

	// Parse SQL text into an AST. No logic implemented here.
	Ast parse(const std::string &sql) const;
};

} // namespace spl

#endif // SPL_SQLPARSER_H