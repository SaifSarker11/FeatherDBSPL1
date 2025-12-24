#ifndef SPL_VALIDATORS_H
#define SPL_VALIDATORS_H

#include <string>

namespace spl {

class Validators {
public:
	Validators() = default;
	virtual ~Validators() = default;

	bool validateIdentifier(const std::string &id) const;
};

} // namespace spl

#endif // SPL_VALIDATORS_H