#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H

#include <string>
#include <vector>
#include "StorageStructs.h"

namespace spl {

class StorageManager {
public:
    static bool createTable(const std::string& tableName, const std::vector<Column>& columns);
    static Table loadTable(const std::string& tableName);
    static bool saveTable(const Table& table);
    static bool appendRow(const std::string& tableName, const Row& row);
    static bool dropTable(const std::string& tableName);
    static std::vector<std::string> listTables();
    static Table getTableSchema(const std::string& tableName);
};

} // namespace spl

#endif // STORAGE_MANAGER_H
