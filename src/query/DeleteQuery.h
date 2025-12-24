#ifndef SPL_DELETEQUERY_H
#define SPL_DELETEQUERY_H

namespace spl {

class DeleteQuery : public Query {
public:
	DeleteQuery() = default;
	~DeleteQuery() override = default;

	bool execute() const override;
};

} // namespace spl

#endif // SPL_DELETEQUERY_H