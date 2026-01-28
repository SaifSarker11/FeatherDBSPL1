#include <iostream>
#include <set>

#include "QueryExecutor.h"
#include "../storage/StorageManager.h"
#include "../parser/Tokenizer.h"
#include "../parser/SQLParser.h"


namespace spl {

bool isInteger(const std::string& s) {
    if(s.empty()) return false;
    size_t start = 0;
    if(s[0] == '-' || s[0] == '+') {
         if(s.size() == 1) return false;
         start = 1;
    }
    for (size_t i = start; i < s.size(); ++i) {
        if (!std::isdigit(static_cast<unsigned char>(s[i]))) return false;
    }
    return true;
}

int getColumnIndex(const Table& table, const std::string& colName) {
    for (size_t i = 0; i < table.columns.size(); ++i) {
        if (table.columns[i].name == colName) return i;
    }
    return -1;
}

bool evaluateSimple(const Row& row, const Table& table, const std::string& condition) {
    std::stringstream ss(condition);
    std::string colName, op, val;
    ss >> colName >> op >> val; 
    
    if (colName.empty() || op.empty() || val.empty()) return true; // Malformed/Empty condition? Treat as true? Or false.

    int idx = getColumnIndex(table, colName);
    if (idx == -1) return false; 

    std::string rowVal = row.values[idx];

    // Remove quotes
    if (val.size() >= 2 && val.front() == '\'' && val.back() == '\'') {
        val = val.substr(1, val.size() - 2);
    }
    
    if (table.columns[idx].type == "INT" || table.columns[idx].type == "int") {
        try {
            int r = std::stoi(rowVal);
            int v = std::stoi(val);
            if (op == "=") return r == v;
            if (op == ">") return r > v;
            if (op == "<") return r < v;
            if (op == ">=") return r >= v;
            if (op == "<=") return r <= v;
            if (op == "!=") return r != v;
        } catch (...) { return false; }
    } else {
        if (op == "=") return rowVal == val;
        if (op == "!=") return rowVal != val;
        if (op == ">") return rowVal > val;
        if (op == "<") return rowVal < val;
    }
    return false;
}

void QueryExecutor::execute(std::unique_ptr<AST> ast) {
    if (!ast) return;
    if (ast->type == "CREATE") {
        handleCreate(static_cast<CreateStatement*>(ast.get()));
    } else if (ast->type == "INSERT") {
        handleInsert(static_cast<InsertStatement*>(ast.get()));
    } else if (ast->type == "SELECT") {
         Table t = executeSelect(static_cast<SelectStatement*>(ast.get()));
         t.print();
    } else if (ast->type == "UPDATE") {
        handleUpdate(static_cast<UpdateStatement*>(ast.get()));
    } else if (ast->type == "DELETE") {
        handleDelete(static_cast<DeleteStatement*>(ast.get()));
    } else {
        std::cout << "Unknown query type: " << ast->type << "\n";
    }
}

void QueryExecutor::handleCreate(CreateStatement* stmt) {
    std::vector<Column> cols;
    for (const auto& p : stmt->columns) {
        cols.push_back({p.first, p.second});
    }
    if (StorageManager::createTable(stmt->table, cols)) {
        std::cout << "Table '" << stmt->table << "' created.\n";
    } else {
        std::cout << "Error: Table '" << stmt->table << "' already exists or create failed.\n";
    }
}

void QueryExecutor::handleInsert(InsertStatement* stmt) {
    Table table = StorageManager::getTableSchema(stmt->table);
    if (table.columns.empty()) {
         std::cout << "Error: Table '" << stmt->table << "' not found.\n";
         return;
    }

    if (stmt->values.size() != table.columns.size()) {
         std::cout << "Error: Column count mismatch.\n";
         return;
    }

    for (size_t i = 0; i < table.columns.size(); ++i) {
        std::string val = stmt->values[i];
        std::string type = table.columns[i].type;
        if (type == "INT" || type == "int") {
            if (!isInteger(val)) {
                std::cout << "Error: Invalid INT value '" << val << "' for column '" << table.columns[i].name << "'\n";
                return;
            }
        }
    }

    Row row;
    row.values = stmt->values;
    if (StorageManager::appendRow(stmt->table, row)) {
        std::cout << "1 row inserted.\n";
    } else {
        std::cout << "Error: Could not write to table.\n";
    }
}

// Quicksort Helpers
void swapRows(Row& a, Row& b) {
    Row temp = a;
    a = b;
    b = temp;
}

int partition(std::vector<Row>& rows, int low, int high, int colIdx, bool isInt) {
    std::string pivotStr = rows[high].values[colIdx];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        std::string curStr = rows[j].values[colIdx];
        bool less = false;
        if (isInt) {
            try {
                int p = std::stoi(pivotStr);
                int c = std::stoi(curStr);
                less = (c < p);
            } catch (...) { less = (curStr < pivotStr); }
        } else {
             less = (curStr < pivotStr);
        }

        if (less) {
            i++;
            swapRows(rows[i], rows[j]);
        }
    }
    swapRows(rows[i + 1], rows[high]);
    return (i + 1);
}

void quickSort(std::vector<Row>& rows, int low, int high, int colIdx, bool isInt) {
    if (low < high) {
        int pi = partition(rows, low, high, colIdx, isInt);
        quickSort(rows, low, pi - 1, colIdx, isInt);
        quickSort(rows, pi + 1, high, colIdx, isInt);
    }
}

Table QueryExecutor::executeSelect(SelectStatement* stmt) {
    Table sourceTable;
    if (stmt->nestedFrom) {
        if(stmt->nestedFrom->type == "SELECT") {
             sourceTable = executeSelect(static_cast<SelectStatement*>(stmt->nestedFrom.get()));
             sourceTable.name = "nested"; // anonymous
        } else {
             std::cout << "Error: Nested FROM must be SELECT\n";
             return Table();
        }
    } else {
        sourceTable = StorageManager::loadTable(stmt->table);
    }
    
    if(sourceTable.columns.empty() && sourceTable.name.empty() && !stmt->nestedFrom) {
         std::cout << "Error: Table " << stmt->table << " not found.\n";
         return Table();
    }
    
    Table filteredTable;
    filteredTable.name = sourceTable.name;
    filteredTable.columns = sourceTable.columns;
    
    if (stmt->condition.empty()) {
        filteredTable.rows = sourceTable.rows;
    } else {
        std::string condition = stmt->condition;
        std::set<std::string> inValues;
        std::string inCol;
        bool hasIn = false;
        
        size_t inPos = condition.find(" IN (SELECT");
        if (inPos != std::string::npos) {
             hasIn = true;
             size_t spacePos = condition.rfind(" ", inPos - 1);
             if (spacePos != std::string::npos) {
                 inCol = condition.substr(spacePos + 1, inPos - spacePos - 1);
             } else {
                 inCol = condition.substr(0, inPos);
             }
             
             size_t startParen = condition.find("(", inPos);
             size_t endParen = condition.rfind(")");
             std::string subSQL = condition.substr(startParen + 1, endParen - startParen - 1);
             
             Tokenizer tokenizer(subSQL);
             SQLParser parser(tokenizer);
             std::unique_ptr<AST> subAst = parser.parse();
             QueryExecutor subExec;
             if (subAst && subAst->type == "SELECT") {
                  Table subRes = subExec.executeSelect(static_cast<SelectStatement*>(subAst.get()));
                  for(const auto& r : subRes.rows) {
                      if(!r.values.empty()) inValues.insert(r.values[0]);
                  }
             }
        }
        
        for (const auto& row : sourceTable.rows) {
            bool pass = false;
            if (hasIn) {
                int idx = getColumnIndex(sourceTable, inCol);
                if (idx != -1) {
                    if (inValues.count(row.values[idx])) pass = true;
                }
            } else {
                if (evaluateSimple(row, sourceTable, condition)) pass = true;
            }
            if (pass) filteredTable.rows.push_back(row);
        }
    }
    
    // Sort
    if (!stmt->orderBy.empty()) {
        int sortIdx = getColumnIndex(filteredTable, stmt->orderBy);
        if (sortIdx != -1) {
             bool isInt = (filteredTable.columns[sortIdx].type == "INT" || filteredTable.columns[sortIdx].type == "int");
             if (!filteredTable.rows.empty())
                quickSort(filteredTable.rows, 0, filteredTable.rows.size() - 1, sortIdx, isInt);
        } else {
             std::cout << "Warning: Order By column " << stmt->orderBy << " not found.\n";
        }
    }
    
    if (stmt->columns.size() == 1 && stmt->columns[0] == "*") {
        return filteredTable;
    }
    
    Table resultTable;
    resultTable.name = filteredTable.name;
    std::vector<int> colIndices;
    for(const auto& colName : stmt->columns) {
        int idx = getColumnIndex(filteredTable, colName);
        if(idx != -1) {
            resultTable.columns.push_back(filteredTable.columns[idx]);
            colIndices.push_back(idx);
        } else {
             // Ignore unknown columns or handle? AST parser puts them in.
        }
    }
    
    for(const auto& row : filteredTable.rows) {
        Row newRow;
        for(int idx : colIndices) {
            newRow.values.push_back(row.values[idx]);
        }
        resultTable.rows.push_back(newRow);
    }
    
    return resultTable;
}

void QueryExecutor::handleUpdate(UpdateStatement* stmt) {
    Table table = StorageManager::loadTable(stmt->table);
    if (table.columns.empty()) {
        std::cout << "Error: Table " << stmt->table << " not found.\n";
        return;
    }
    
    int setIdx = getColumnIndex(table, stmt->column);
    if (setIdx == -1) {
         std::cout << "Error: Column " << stmt->column << " not found.\n";
         return;
    }
    
    int count = 0;
    for (auto& row : table.rows) {
        if (stmt->condition.empty() || evaluateSimple(row, table, stmt->condition)) {
            row.values[setIdx] = stmt->value;
            count++;
        }
    }
    
    if (StorageManager::saveTable(table)) {
        std::cout << count << " rows updated.\n";
    } else {
        std::cout << "Error saving table.\n";
    }
}

void QueryExecutor::handleDelete(DeleteStatement* stmt) {
    Table table = StorageManager::loadTable(stmt->table);
    if (table.columns.empty()) {
        std::cout << "Error: Table " << stmt->table << " not found.\n";
        return;
    }
    
    Table newTable;
    newTable.name = table.name;
    newTable.columns = table.columns;
    
    int count = 0;
    for (const auto& row : table.rows) {
        if (!stmt->condition.empty() && evaluateSimple(row, table, stmt->condition)) {
            count++; // Skip (delete)
        } else {
            newTable.rows.push_back(row);
        }
    }
    
    if (StorageManager::saveTable(newTable)) {
        std::cout << count << " rows deleted.\n";
    } else {
         std::cout << "Error saving table.\n";
    }
}

void QueryExecutor::handleSelect(SelectStatement* stmt) {
    executeSelect(stmt).print();
}

} // namespace spl