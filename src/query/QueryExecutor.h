#ifndef SPL_QUERYEXECUTOR_H
#define SPL_QUERYEXECUTOR_H

#include <memory>

namespace spl {

class Query;

class QueryExecutor {
public:
	QueryExecutor() = default;
	virtual ~QueryExecutor() = default;

	bool execute(const Query &q);
};

} // namespace spl

#endif // SPL_QUERYEXECUTOR_H