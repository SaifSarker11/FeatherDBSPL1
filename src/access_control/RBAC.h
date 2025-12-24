#ifndef SPL_RBAC_H
#define SPL_RBAC_H

#include <string>

namespace spl {

class Rbac {
public:
	Rbac() = default;
	virtual ~Rbac() = default;

	// Lightweight interface: check permission for a user/action
	bool hasPermission(const std::string &user, const std::string &action) const;
};

} // namespace spl
