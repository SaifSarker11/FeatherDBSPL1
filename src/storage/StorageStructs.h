#ifndef STORAGE_STRUCTS_H
#define STORAGE_STRUCTS_H

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

namespace spl {

struct Column {
    std::string name;
    std::string type; // "INT", "STRING"
};

struct Row {
    std::vector<std::string> values;
};

class Table {
public:
    std::string name;
    std::vector<Column> columns;
    std::vector<Row> rows;

    void print() const {
        // Simple pretty print
        for (const auto& col : columns) {
            std::cout << std::left << std::setw(15) << col.name;
        }
        std::cout << "\n";
        for (const auto& col : columns) {
            std::cout << "---------------";
        }
        std::cout << "\n";
        for (const auto& row : rows) {
            for (const auto& val : row.values) {
                std::cout << std::left << std::setw(15) << val;
            }
            std::cout << "\n";
        }
    }
};

} // namespace spl

#endif // STORAGE_STRUCTS_H
