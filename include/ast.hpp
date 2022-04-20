#ifndef ast_hpp
#define ast_hpp

#include <vector>

#include "ast/ast_expression.hpp"
#include "ast/ast_function.hpp"
#include "ast/ast_statement.hpp"
#include "ast/ast_primary_expression.hpp"

extern void yyset_in(FILE* fd);
extern std::vector<const Node *> parse_AST();

#endif
