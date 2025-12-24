#ifndef SPL_FILEMANAGER_H
#define SPL_FILEMANAGER_H

#include <string>

namespace spl {

class FileManager {
public:
	FileManager() = default;
	virtual ~FileManager() = default;

	bool write(const std::string &path, const std::string &data);
	std::string read(const std::string &path) const;
};

} // namespace spl

#endif // SPL_FILEMANAGER_H