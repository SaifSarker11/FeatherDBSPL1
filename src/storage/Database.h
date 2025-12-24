#ifndef SPL_DATABASE_H
#define SPL_DATABASE_H

#include <string>

namespace spl {

class Database {
public:
	Database() = default;
	virtual ~Database() = default;

	bool open(const std::string &path);
	void close();
};

} // namespace spl

#endif // SPL_DATABASE_H