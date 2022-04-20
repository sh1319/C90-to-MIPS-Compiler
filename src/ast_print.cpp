#include "../include/ast.hpp"
#include "../include/ast_traverse.hpp"
#include "../include/generate_assembly.hpp"

int main() {
    Symbol_Table *symbol_table = new Symbol_Table();
    std::vector<const Node*> roots = parse_AST();
    for (const Node* ast : roots) {
        traverse_AST_print(std::cerr, ast, symbol_table);
        ast->print(std::cerr, " ");
    }
    symbol_table->display();
    //generate_global_variable(symbol_table);
    //generate_function_frame(symbol_table);
    return 0;
}