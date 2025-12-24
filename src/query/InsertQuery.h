#ifndef SPL_INSERTQUERY_H
#define SPL_INSERTQUERY_H

namespace spl {

class InsertQuery : public Query {
public:
	InsertQuery() = default;
	~InsertQuery() override = default;

	bool execute() const override;
};

} // namespace spl

#endif // SPL_INSERTQUERY_H