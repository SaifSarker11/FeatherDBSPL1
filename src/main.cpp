#include "parser/Tokenizer.h"
#include "parser/SQLParser.h"
#include "query/QueryExecutor.h"
#include "storage/StorageManager.h"
#include "utils/Print.h"

using namespace spl;


int main()
{
    std::cout << "FeatherDB v1.0.0\n";
    std::cout << "Type .help for instructions.\n";

    std::string input;
    while (true)
    {
        std::cout << "featherdb> ";
        std::getline(std::cin, input);

        if (input.empty())
            continue;

        if (input[0] == '.')
        {
            if (input == ".exit")
            {
                break;
            }
            else if (input == ".help")
            {
                printHelp();
            }
            else if (input == ".tables")
            {
                std::vector<std::string> tables = StorageManager::listTables();
                for (const auto &t : tables)
                {
                    std::cout << t << "\n";
                }
            }
            else if (input.rfind(".schema", 0) == 0)
            {
                // extract table name
                std::stringstream ss(input);
                std::string cmd, name;
                ss >> cmd >> name;
                if (name.empty())
                {
                    std::cout << "Usage: .schema <table_name>\n";
                }
                else
                {
                    Table t = StorageManager::getTableSchema(name);
                    if (t.name.empty() && t.columns.empty())
                    {
                        std::cout << "Table '" << name << "' not found.\n";
                    }
                    else
                    {
                        std::cout << "CREATE TABLE " << t.name << " (";
                        for (size_t i = 0; i < t.columns.size(); ++i)
                        {
                            std::cout << t.columns[i].name << " " << t.columns[i].type;
                            if (i < t.columns.size() - 1)
                                std::cout << ", ";
                        }
                        std::cout << ")\n";
                    }
                }
            }
            else
            {
                std::cout << "Unknown command: " << input << "\n";
            }
            continue;
        }

        // SQL Execution
        try
        {
            Tokenizer tokenizer(input);
            SQLParser parser(tokenizer);
            std::unique_ptr<AST> ast = parser.parse();

            QueryExecutor executor;
            executor.execute(std::move(ast));
        }
        catch (const std::exception &e)
        {
            std::cout << "Error: " << e.what() << "\n";
        }
    }

    return 0;
}
