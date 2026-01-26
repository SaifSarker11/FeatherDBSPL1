#ifndef SPL_QUERYEXECUTOR_H
#define SPL_QUERYEXECUTOR_H

#include <memory>
#include "../parser/AST.h"
#include "../storage/StorageStructs.h"

namespace spl {

class QueryExecutor {
public:
	QueryExecutor() = default;
	virtual ~QueryExecutor() = default;

	// Main entry point
	void execute(std::unique_ptr<AST> ast);

private:
	void handleCreate(CreateStatement* stmt);
	void handleInsert(InsertStatement* stmt);
	void handleSelect(SelectStatement* stmt); // Changed to void for now, will print result or return Table later.
    // Making it return Table is better for nested queries.
    Table executeSelect(SelectStatement* stmt);

	void handleUpdate(UpdateStatement* stmt);
	void handleDelete(DeleteStatement* stmt);
};

} // namespace spl

#endif // SPL_QUERYEXECUTOR_H