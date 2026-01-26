#include "StorageManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

namespace spl {

namespace fs = std::filesystem;

bool StorageManager::createTable(const std::string& tableName, const std::vector<Column>& columns) {
    if (!fs::exists("db")) {
        fs::create_directory("db");
    }
    std::string pathPrefix = "db/" + tableName;

    if (fs::exists(pathPrefix + ".schema")) {
        return false; // Already exists
    }

    std::ofstream schemaFile(pathPrefix + ".schema");
    if (!schemaFile.is_open()) return false;

    for (const auto& col : columns) {
        schemaFile << col.name << " " << col.type << "\n";
    }
    schemaFile.close();

    std::ofstream dataFile(pathPrefix + ".csv"); // Empty data file
    if (!dataFile.is_open()) return false;
    dataFile.close();

    return true;
}

Table StorageManager::loadTable(const std::string& tableName) {
    Table table;
    table.name = tableName;
    std::string pathPrefix = "db/" + tableName;

    // Load Schema
    std::ifstream schemaFile(pathPrefix + ".schema");
    if (!schemaFile.is_open()) {
        return table;
    }

    std::string line;
    while (std::getline(schemaFile, line)) {
        std::stringstream ss(line);
        std::string name, type;
        ss >> name >> type;
        if (!name.empty()) {
            table.columns.push_back({name, type});
        }
    }
    schemaFile.close();

    // Load Data
    std::ifstream dataFile(pathPrefix + ".csv");
    if (dataFile.is_open()) {
        while (std::getline(dataFile, line)) {
            // Simple CSV split
            Row row;
            std::stringstream ss(line);
            std::string cell;
            while (std::getline(ss, cell, ',')) {
                 row.values.push_back(cell);
            }
            if (!row.values.empty()) {
                 table.rows.push_back(row);
            }
        }
        dataFile.close();
    }

    return table;
}

bool StorageManager::saveTable(const Table& table) {
    if (!fs::exists("db")) {
        fs::create_directory("db");
    }
    std::string pathPrefix = "db/" + table.name;
    std::ofstream dataFile(pathPrefix + ".csv");
    if (!dataFile.is_open()) return false;

    for (const auto& row : table.rows) {
        for (size_t i = 0; i < row.values.size(); ++i) {
            dataFile << row.values[i];
            if (i < row.values.size() - 1) {
                dataFile << ",";
            }
        }
        dataFile << "\n";
    }
    dataFile.close();
    return true;
}

bool StorageManager::appendRow(const std::string& tableName, const Row& row) {
    if (!fs::exists("db")) {
        fs::create_directory("db");
    }
    std::string pathPrefix = "db/" + tableName;
    std::ofstream dataFile(pathPrefix + ".csv", std::ios::app);
    if (!dataFile.is_open()) return false;

    for (size_t i = 0; i < row.values.size(); ++i) {
        dataFile << row.values[i];
        if (i < row.values.size() - 1) {
            dataFile << ",";
        }
    }
    dataFile << "\n";
    dataFile.close();
    return true;
}

bool StorageManager::dropTable(const std::string& tableName) {
    std::string pathPrefix = "db/" + tableName;
    bool s = fs::remove(pathPrefix + ".schema");
    bool d = fs::remove(pathPrefix + ".csv");
    return s && d;
}

std::vector<std::string> StorageManager::listTables() {
    std::vector<std::string> tables;
    if (!fs::exists("db")) return tables;
    
    for (const auto& entry : fs::directory_iterator("db")) {
        if (entry.path().extension() == ".schema") {
            tables.push_back(entry.path().stem().string());
        }
    }
    return tables;
}

Table StorageManager::getTableSchema(const std::string& tableName) {
     Table table;
    table.name = tableName;
    std::string pathPrefix = "db/" + tableName;

    // Load Schema Only
    std::ifstream schemaFile(pathPrefix + ".schema");
    if (!schemaFile.is_open()) {
        return table;
    }

    std::string line;
    while (std::getline(schemaFile, line)) {
        std::stringstream ss(line);
        std::string name, type;
        ss >> name >> type;
        if (!name.empty()) {
            table.columns.push_back({name, type});
        }
    }
    schemaFile.close();
    return table;
}

} // namespace spl
