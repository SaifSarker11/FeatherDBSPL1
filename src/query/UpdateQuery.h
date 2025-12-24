
#ifndef SPL_UPDATEQUERY_H
#define SPL_UPDATEQUERY_H

namespace spl {

class UpdateQuery : public Query {
public:
	UpdateQuery() = default;
	~UpdateQuery() override = default;

	bool execute() const override;
};

} // namespace spl

#endif // SPL_UPDATEQUERY_H
