#ifndef SPL_SELECTQUERY_H
#define SPL_SELECTQUERY_H

#include <string>

namespace spl {

class Query {
public:
	Query() = default;
	virtual ~Query() = default;
	virtual bool execute() const = 0;
};

class SelectQuery : public Query {
public:
	SelectQuery() = default;
	~SelectQuery() override = default;

	bool execute() const override;
};

} // namespace spl

#endif // SPL_SELECTQUERY_H