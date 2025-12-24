#ifndef SPL_STORAGEENGINE_H
#define SPL_STORAGEENGINE_H

namespace spl {

class StorageEngine {
public:
	StorageEngine() = default;
	virtual ~StorageEngine() = default;

	void start();
	void stop();
};

} // namespace spl

#endif // SPL_STORAGEENGINE_H