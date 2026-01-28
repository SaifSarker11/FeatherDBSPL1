#include "Print.h"
void printHelp()
{
    std::cout << "FeatherDB Meta-Commands:\n";
    std::cout << "  .help            Show this help message\n";
    std::cout << "  .exit            Exit the database\n";
    std::cout << "  .tables          List all tables\n";
    std::cout << "  .schema <table>  Show schema for a table\n";
}

void printPrompt(){
    std::cout << "featherDb> ";
}