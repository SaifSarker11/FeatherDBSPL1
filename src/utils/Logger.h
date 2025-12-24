
#ifndef SPL_LOGGERS_H
#define SPL_LOGGERS_H

#include <string>

namespace spl {

class Logger {
public:
	Logger() = default;
	virtual ~Logger() = default;

	void info(const std::string &msg);
	void error(const std::string &msg);
};

} // namespace spl

#endif // SPL_LOGGERS_H
