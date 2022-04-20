#ifndef COMPILE_HELPER_HPP
#define COMPILE_HELPER_HPP

#include "ast.hpp"

#include <unordered_map>
#include <unordered_set>
#include <stack>

unsigned int counter = 0;

class Helper {
    private:
    public:
        static void extract_param_names(const Parameter_List *param_list, std::vector<std::string>& param_names) {
            const Parameter_List *param_list_list = dynamic_cast<const Parameter_List *>(param_list->what_param_list());
            if (param_list_list == nullptr) {
                const std::string& variable_id = dynamic_cast<const Variable *>(dynamic_cast<const Parameter_Declaration *>(param_list->what_param())->what_declarator())->what_var();
                param_names.push_back(variable_id);
            }
            else {
                extract_param_names(param_list_list, param_names);
                const Parameter_Declaration *param_declaration = dynamic_cast<const Parameter_Declaration *>(param_list->what_param());
                const std::string& variable_id = dynamic_cast<const Variable*>(param_declaration->what_declarator())->what_var();
                param_names.push_back(variable_id);
            }
        }
        static const std::string empty_param;
        static std::string generate_unique_id(const std::string& name) {
            return "L_" + name + std::to_string(counter++);
        }
        static std::vector<std::string> what_params_func_call(const Parameter_List *param_list){
            std::vector<std::string> param_names;
            extract_param_names(param_list, param_names);
            for(int i = param_names.size(); i < 4; i++) {
                param_names.push_back(empty_param);
            }
            return param_names;
        }
        static int interpret_constant_expr(const Node* expr) {
            if (expr->what_type() == "Int_Constant") {
                std::cerr << "[interpret_constant_expr]: int constant" << std::endl;
                const Int_Constant* int_constant = dynamic_cast<const Int_Constant *>(expr);
                return int_constant->what_val();
            }
            else if (expr->what_type() == "Variable_Normal") {
                // TODO: global variable initialised as previously initialised global variable
            }
            else if (expr->what_type() == "Unary_Expression") {
                const Unary_Expression *unary_expr = dynamic_cast<const Unary_Expression *>(expr);
                int tmp = interpret_constant_expr(unary_expr->what_unary_expr());
                if (unary_expr->what_op() == "-") { return -tmp; }
                else if (unary_expr->what_op() == "~") {return ~tmp; }
                else if (unary_expr->what_op() == "!") { return !tmp; }
            }
            else if (expr->what_type() == "Multiplicative_Expression") {
                const Multiplicative_Expression *mult_expr = dynamic_cast<const Multiplicative_Expression *>(expr);
                int lhs = interpret_constant_expr(mult_expr->what_left());
                int rhs = interpret_constant_expr(mult_expr->what_right());
                if (mult_expr->what_op() == "*") { return lhs * rhs; }
                else if (mult_expr->what_op() == "/") { return lhs / rhs; }
                else if (mult_expr->what_op() == "%") { return lhs % rhs; }
            }
            else if (expr->what_type() == "Additive_Expression") {
                const Additive_Expression *add_expr = dynamic_cast<const Additive_Expression *>(expr);
                int lhs = interpret_constant_expr(add_expr->what_left());
                int rhs = interpret_constant_expr(add_expr->what_right());
                if (add_expr->what_op() == "+") { return lhs + rhs; }
                else if (add_expr->what_op() == "-") { return lhs - rhs; }
            }
            else if (expr->what_type() == "Shift_Expression") {
                const Shift_Expression *shift_expr = dynamic_cast<const Shift_Expression *>(expr);
                int lhs = interpret_constant_expr(shift_expr->what_left());
                int rhs = interpret_constant_expr(shift_expr->what_right());
                if (shift_expr->what_op() == "<<") { return lhs << rhs; }  
                else if (shift_expr->what_op() == ">>") { return lhs >> rhs; }
            }
            else if (expr->what_type() == "Relational_Expression") {
                const Relational_Expression *relational_expr = dynamic_cast<const Relational_Expression *>(expr);
                int lhs = interpret_constant_expr(relational_expr->what_left());
                int rhs = interpret_constant_expr(relational_expr->what_right());
                if (relational_expr->what_op() == "<") { return lhs < rhs; }  
                else if (relational_expr->what_op() == ">") { return lhs > rhs; }  
                else if (relational_expr->what_op() == "<=") { return lhs <= rhs; }  
                else if (relational_expr->what_op() == ">=") { return lhs >= rhs; }
            }

            else if (expr->what_type() == "Equality_Expression") {
                const Equality_Expression *equality_expression = dynamic_cast<const Equality_Expression *>(expr);
                int lhs = interpret_constant_expr(equality_expression->what_left());
                int rhs = interpret_constant_expr(equality_expression->what_right());
                if (equality_expression->what_op() == "==") { return lhs == rhs; }  
                else if (equality_expression->what_op() == "!=") { return lhs != rhs; }
            }
            else if (expr->what_type() == "And_Expression") {
                const And_Expression *and_expr = dynamic_cast<const And_Expression *>(expr);
                int lhs = interpret_constant_expr(and_expr->what_left());
                int rhs = interpret_constant_expr(and_expr->what_right());
                return lhs & rhs;  
            }
            else if (expr->what_type() == "Exclusive_Or_Expression") {
                const Exclusive_Or_Expression *exclusive_or_expr = dynamic_cast<const Exclusive_Or_Expression *>(expr);
                int lhs = interpret_constant_expr(exclusive_or_expr->what_left());
                int rhs = interpret_constant_expr(exclusive_or_expr->what_right());
                return lhs ^ rhs;
            }
            else if (expr->what_type() == "Inclusive_Or_Expression") {
                const Inclusive_Or_Expression *inclusive_or_expr = dynamic_cast<const Inclusive_Or_Expression *>(expr);
                int lhs = interpret_constant_expr(inclusive_or_expr->what_left());
                int rhs = interpret_constant_expr(inclusive_or_expr->what_right());
                return lhs | rhs;
            }
            else if (expr->what_type() == "Logical_And_Expression") {
                const Logical_And_Expression *logical_and_expression = dynamic_cast<const Logical_And_Expression*>(expr);
                int lhs = interpret_constant_expr(logical_and_expression->what_left());
                int rhs = interpret_constant_expr(logical_and_expression->what_right());
                return lhs && rhs;
            }
            else if (expr->what_type() == "Logical_Or_Expression") {
                const Logical_Or_Expression *logical_or_expr = dynamic_cast<const Logical_Or_Expression*>(expr);
                int lhs = interpret_constant_expr(logical_or_expr->what_left());
                int rhs = interpret_constant_expr(logical_or_expr->what_right());
                return lhs || rhs;
            }
            else if (expr->what_type() == "Conditional_Expression") {
                const Conditional_Expression *conditional_expression = dynamic_cast<const Conditional_Expression *>(expr);
                int cond = interpret_constant_expr(conditional_expression->what_cond());
                int expr1 = interpret_constant_expr(conditional_expression->what_expr_true());
                int expr2 = interpret_constant_expr(conditional_expression->what_expr_false());
                return cond ? expr1 : expr2;
            }
        }
        static int what_stack_frame_size(const Node* ast_node) {
            const std::string& node_type = ast_node->what_type();
            if (node_type == "Function_Definition") {
                std::cerr << "[what_stack_frame_size]: function_definition" << std::endl;
                return what_stack_frame_size(dynamic_cast<const Function_Definition*>(ast_node)->what_definition());
            }
            else if (node_type == "Statement_List") {
                std::cerr << "[what_stack_frame_size]: statement_list" << std::endl;
                const Statement_List *stmt_list = dynamic_cast<const Statement_List *>(ast_node);
                if(stmt_list->what_stmt() == nullptr) {
                    return 0;
                } 
                else if (stmt_list->what_stmt_list() == nullptr) {
                    return what_stack_frame_size(stmt_list->what_stmt());
                } 
                else if (stmt_list->what_stmt_list() != nullptr) {
                    return what_stack_frame_size(stmt_list->what_stmt()) + what_stack_frame_size(stmt_list->what_stmt_list());
                }
            }
            else if (node_type == "Compound_Statement") {
                std::cerr << "[what_stack_frame_size]: compound_stmt" << std::endl;
                const Compound_Statement *compound_stmt = dynamic_cast<const Compound_Statement *>(ast_node);
                if (compound_stmt->what_stmt_list() != nullptr && compound_stmt->what_decl_list() != nullptr) {
                    return what_stack_frame_size(compound_stmt->what_stmt_list()) + what_stack_frame_size(compound_stmt->what_decl_list());
                }
                else if (compound_stmt->what_stmt_list() != nullptr) {
                    return what_stack_frame_size(compound_stmt->what_stmt_list());
                }
                else if (compound_stmt->what_decl_list() != nullptr) {
                    return what_stack_frame_size(compound_stmt->what_decl_list());
                }
                else {
                    return 0;
                }
            }
            else if (node_type == "While_Statement") {
                std::cerr << "[what_stack_frame_size]: while_stmt" << std::endl;
                return what_stack_frame_size(dynamic_cast<const While_Statement*>(ast_node)->what_stmt());
            }
            else if (node_type == "If_Statement") {
                std::cerr << "[what_stack_frame_size]: if_stmt" << std::endl;
                const If_Statement *if_stmt = dynamic_cast<const If_Statement *>(ast_node);
                if (if_stmt->what_stmt_else() == nullptr) { return what_stack_frame_size(if_stmt->what_stmt_if()); } 
                else { return what_stack_frame_size(if_stmt->what_stmt_if()) + what_stack_frame_size(if_stmt->what_stmt_else()); }
            }
            else if (node_type == "Declaration_List") {
                std::cerr << "[what_stack_frame_size]: declaration_list" << std::endl;
                const Declaration_List* decl_list = dynamic_cast<const Declaration_List *>(ast_node);
                const Declaration_List *decl_list_list = dynamic_cast<const Declaration_List *>(decl_list->what_decl_list());
                int bytes = 8;
                if (decl_list_list != nullptr) {
                    const Declaration *decl_list_decl = dynamic_cast<const Declaration *>(decl_list->what_decl());
                    bytes += what_stack_frame_size(decl_list_list);
                    bytes += what_stack_frame_size(decl_list_decl);
                }
                else {
                    const Declaration *decl_list_decl = dynamic_cast<const Declaration *>(decl_list->what_decl());
                    bytes += what_stack_frame_size(decl_list_decl);
                }
                return bytes;
            }
            else if (node_type == "Declaration") {
                std::cerr << "[what_stack_frame_size]: declaration" << std::endl;
                const Declaration *decl = dynamic_cast<const Declaration *>(ast_node);
                int bytes = 4;
                const Declaration_Specifiers *decl_specifiers = dynamic_cast<const Declaration_Specifiers *>(decl->what_decl_specifier());
                if(decl_specifiers->what_decl_specifiers() == "int") {
                    const Declarator *decl_declarator = dynamic_cast<const Declarator *>(dynamic_cast<const Init_Declarator_List *>(decl->what_init_declarator_list())->what_init_declarator());
                    const Variable *var = dynamic_cast<const Variable *>(decl_declarator->what_declarator());
                    if (var->what_type() == "Variable_Normal" || var->what_type() == "Variable_Pointer") {
                        bytes += 4;
                    } 
                    else if (var->what_type() == "Variable_Array") {
                        bytes += Helper::interpret_constant_expr(var->what_index_array())*4;
                    } 
                }
                else {
                    // TODO: FLOATS
                }
                return bytes;
            }
            else { return 40; }
        }
};

const std::string Helper::empty_param = "NO_FUNC_PARAM";

#endif