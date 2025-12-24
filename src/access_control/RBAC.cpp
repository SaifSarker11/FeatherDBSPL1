#include "Rbac.h"

namespace spl {

bool Rbac::hasPermission(const std::string & /*user*/, const std::string & /*action*/) const {
	// Intentionally minimal: no logic here (stub)
	return false;
}

} // namespace spl