#include <iostream>
#include <fstream>
#include <string>

#include "../include/generate_assembly.hpp"

int main(int argc, char** argv) {
    std::cerr << "Let's start compiling" << std::endl;
    if (argc == 5 && std::string(argv[1]) == "-S" && std::string(argv[3]) == "-o") {
        std::cerr << "Parsing" << std::endl;
        FILE* file_in;
        file_in = fopen(argv[2], "r");
        yyset_in(file_in);

        std::vector<const Node *> roots = parse_AST();
        std::cerr << "AST built" << std::endl;
        std::ofstream mips;
        std::string file = argv[4];
        mips.open(file);
        std::cerr << "Open file" << std::endl;
        Symbol_Table *symbol_table = new Symbol_Table();
        Register regs = Register();
        std::cerr << "Traversing the AST" << std::endl;
        for (const Node* ast : roots) {
            traverse_AST_print(std::cerr, std::cout, ast, symbol_table);
            //ast->print(std::cerr, " ");
        } 
        symbol_table->display();
        c_to_mips(mips, roots, symbol_table, regs);
        mips.close();
        std::cerr << "Compiled" << std::endl;
    }
    else {
        std::cerr << argc << " " << argv[1] << " " << argv[2] << argv[3] << " " << argv[4] << std::endl;
        std::cerr << "Invalid Usage: bin/c_compiler -S [source_file] -o [destination_file]" << std::endl;
        return 1;
    }
}
