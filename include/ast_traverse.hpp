#ifndef ast_traverse_hpp
#define ast_traverse_hpp

#include "ast/ast_node.hpp"
#include "symbol_table.hpp"

int count = 0;
int depth = 0;
bool is_break_curr = false;
bool is_break_prev = false;
bool is_default = false;

void traverse_AST_print(std::ostream &dst, std::ostream &asmt, const Node *node, Symbol_Table *symbol_table) {

    /* Subclasses of Node needed to traverse each branch of different nodes. */
    Variable *var_node;
    Int_Constant *int_const_node;
    Float_Constant *float_const_node;
    Char_Constant *char_const_node;
    //String_Constant *string_const_node; NOT IMPLEMENTED
    
    Statement_List *stmt_list_node;
    Compound_Statement *compound_stmt_node;
    While_Statement *while_stmt_node;
    For_Statement *for_stmt_node;
    If_Statement *if_stmt_node;
    Return_Statement *return_stmt_node;

    Declaration_List *decl_list_node;
    Declaration *decl_node;
    Init_Declarator_List *init_decl_list_node;
    Declaration_Specifiers *decl_specifiers_node;
    Declarator *declarator_node;

    Postfix_Expression *postfix_expr_node;
    Conditional_Expression *cond_expr_node;
    Assignment_Expression *assign_expr_node;
    //Empty_Expression *empty_expr_node; EMPTY SO NOT NEEDED BUT LEFT AS REFERENCE

    Unary_Expression *unary_expr_node;
    Multiplicative_Expression *mult_expr_node;
    Additive_Expression *add_expr_node;
    Shift_Expression *shift_expr_node;
    Relational_Expression *relational_expr_node;
    Equality_Expression *equality_expr_node;

    And_Expression *and_expr_node;
    Exclusive_Or_Expression *excl_or_expr_node;
    Inclusive_Or_Expression *incl_or_expr_node;
    Logical_And_Expression *logic_and_expr_node;
    Logical_Or_Expression *logic_or_expr_node;

    Function_Declaration *func_decl_node;
    Function_Definition *func_def_node;
    Parameter_List *param_list_node;
    Parameter_Declaration *param_decl_node;

    Function_Call *func_call_node;
    Argument_List *arg_list_node;

    int i = count;

    //print node type
    if(node != nullptr) { while(i--) { dst << "\t"; } dst << "|<" << node->what_type() << ">" << std::endl; }
    
    //pre-order traversal and printing nodes and values
    if (node == nullptr) { int j = count; while(j--) { dst << "\t"; } dst << "|nullptr" << std::endl; count--; }
    else if (node->what_type() == "Variable_Normal") { 
        var_node = (Variable *) node;
        int j = count + 1;
        while(j--) { dst << "\t"; }
        dst << "|" << var_node->what_var() << std::endl; 
        count--;
        if (symbol_table->find(var_node->what_var()) == "-1") {
            symbol_table->insert(var_node->what_var(), "unknown", "unknown", "unknown", -1);
        }
    }
    else if (node->what_type() == "Variable_Pointer") { 
        var_node = (Variable *) node;
        int j = count + 1;
        while(j--) { dst << "\t"; }
        dst << "|" << var_node->what_var() << std::endl; 
        count--;
        if (symbol_table->find(var_node->what_var()) == "-1") {
            symbol_table->insert(var_node->what_var(), "unknown", "unknown", "unknown", -1);
        }
    }
    else if (node->what_type() == "Variable_Array") { 
        var_node = (Variable *) node;
        int j = count + 1;
        while(j--) { dst << "\t"; }
        dst << "|" << var_node->what_var() << std::endl; 
        count--;
        if (symbol_table->find(var_node->what_var()) == "-1") {
            symbol_table->insert(var_node->what_var(), "unknown", "unknown", "unknown", -1);
        }
    }
    else if (node->what_type() == "Int_Constant") {
        int_const_node = (Int_Constant *) node; 
        int j = count + 1;
        while(j--) { dst << "\t"; }
        dst << "|" << int_const_node->what_val() << std::endl; 
        count--;
    }
    else if (node->what_type() == "Float_Constant") { 
        float_const_node = (Float_Constant *) node; 
        int j = count + 1;
        while(j--) { dst << "\t"; }
        dst << float_const_node->what_val() << std::endl;
        count--;
    }
    else if (node->what_type() == "Char_Constant") { 
        char_const_node = (Char_Constant *) node; 
        int j = count + 1;
        while(j--) { dst << "\t"; }
        dst << char_const_node->what_val() << std::endl;
        count--;
    }
    else if (node->what_type() == "String_Constant") { throw std::runtime_error("[Error]: String_Constant Not Implemented"); }
    else if (node->what_type() == "Statement_List") { 
        stmt_list_node = (Statement_List *) node;                                   count++;
        traverse_AST_print(dst, asmt, stmt_list_node->what_stmt_list(), symbol_table);    count++;
        traverse_AST_print(dst, asmt, stmt_list_node->what_stmt(), symbol_table);         count=0;
    }
    else if (node->what_type() == "Compound_Statement") { 
        compound_stmt_node = (Compound_Statement *) node;                           count++;
        traverse_AST_print(dst, asmt, compound_stmt_node->what_stmt_list(), symbol_table);count++;count++;
        traverse_AST_print(dst, asmt, compound_stmt_node->what_decl_list(), symbol_table);count=0;
    }
    else if (node->what_type() == "While_Statement") { 
        while_stmt_node = (While_Statement *) node;             count++;
        traverse_AST_print(dst, asmt, while_stmt_node->what_cond(), symbol_table);        count++;
        traverse_AST_print(dst, asmt, while_stmt_node->what_stmt(), symbol_table);        count=0;
    }
    else if (node->what_type() == "For_Statement") { 
        for_stmt_node = (For_Statement *) node;                 count++;
        traverse_AST_print(dst, asmt, for_stmt_node->what_init(), symbol_table);          count++;
        traverse_AST_print(dst, asmt, for_stmt_node->what_cond(), symbol_table);          count++;
        traverse_AST_print(dst, asmt, for_stmt_node->what_inc(), symbol_table);           count++;
        traverse_AST_print(dst, asmt, for_stmt_node->what_stmt(), symbol_table);          count=0;
    }
    else if (node->what_type() == "If_Statement") { 
        if_stmt_node = (If_Statement *) node;                                       count++;
        traverse_AST_print(dst, asmt, if_stmt_node->what_cond(), symbol_table);           count++;count++;
        traverse_AST_print(dst, asmt, if_stmt_node->what_stmt_if(), symbol_table);        count++;count++;
        traverse_AST_print(dst, asmt, if_stmt_node->what_stmt_else(), symbol_table);      count=0;
    }
    else if (node->what_type() == "Return_Statement") { 
        return_stmt_node = (Return_Statement *) node;                               count++;
        traverse_AST_print(dst, asmt, return_stmt_node->what_expr(), symbol_table);       count=0;
        //asmt << "\tlw\t" << "$2," << symbol_table->what_offset(((Variable *) return_stmt_node->what_expr())->what_var()) << "($fp)" << std::endl;
    }
    else if (node->what_type() == "Break_Statement") { is_break_prev = is_break_curr; is_break_curr = true;}
    else if (node->what_type() == "Continue_Statement") {  }
    else if (node->what_type() == "Switch_Statement") {  }
    else if (node->what_type() == "Case_Statement_List") {  }
    else if (node->what_type() == "Case_Statement") {  }
    else if (node->what_type() == "Default_Statement") { is_break_curr = false; is_default = true; }
    else if (node->what_type() == "Declaration_List") {
        decl_list_node = (Declaration_List *) node;                                 count++;depth++;
        traverse_AST_print(dst, asmt, decl_list_node->what_decl_list(), symbol_table);    count++;
        traverse_AST_print(dst, asmt, decl_list_node->what_decl(), symbol_table);         count=0;

        std::string variable = ((Variable *) ((Declarator *) ((Init_Declarator_List *) ((Declaration *) decl_list_node->what_decl())->what_init_declarator_list())->what_init_declarator())->what_declarator())->what_var();
        std::string var_type = ((Variable *) ((Declarator *) ((Init_Declarator_List *) ((Declaration *) decl_list_node->what_decl())->what_init_declarator_list())->what_init_declarator())->what_declarator())->what_type();
        if (symbol_table->find(variable) == "1") {
            symbol_table->modify(variable, "local var");
        }
    }
    else if (node->what_type() == "Declaration") {
        decl_node = (Declaration *) node;                                               count++;
        traverse_AST_print(dst, asmt, decl_node->what_decl_specifier(), symbol_table);        count++;
        traverse_AST_print(dst, asmt, decl_node->what_init_declarator_list(), symbol_table);  count=0;
        if (((Declarator *) ((Init_Declarator_List *) decl_node->what_init_declarator_list())->what_init_declarator())->what_declarator()->what_type() == "Variable_Normal") {
            std::string variable = ((Variable *) ((Declarator *) ((Init_Declarator_List *) decl_node->what_init_declarator_list())->what_init_declarator())->what_declarator())->what_var();
            std::string var_type = ((Variable *) ((Declarator *) ((Init_Declarator_List *) decl_node->what_init_declarator_list())->what_init_declarator())->what_declarator())->what_type();
            std::string type = ((Declaration_Specifiers *) decl_node->what_decl_specifier())->what_decl_specifiers();
            if (symbol_table->find(variable) == "1") {
                symbol_table->modify(variable, "global var", type);
            }
        }
        else if (((Declarator *) ((Init_Declarator_List *) decl_node->what_init_declarator_list())->what_init_declarator())->what_declarator()->what_type() == "Variable_Array") {
            std::string variable = ((Variable *) ((Declarator *) ((Init_Declarator_List *) decl_node->what_init_declarator_list())->what_init_declarator())->what_declarator())->what_var();
            std::string type = ((Declaration_Specifiers *) decl_node->what_decl_specifier())->what_decl_specifiers();
            if (symbol_table->find(variable) == "1") {
                symbol_table->modify(variable, "global var", type + " arr");
            }
        }
        else if (((Declarator *) ((Init_Declarator_List *) decl_node->what_init_declarator_list())->what_init_declarator())->what_declarator()->what_type() == "Function_Declaration") {
            std::string function = ((Variable *) ((Function_Declaration *) ((Declarator *) ((Init_Declarator_List *) decl_node->what_init_declarator_list())->what_init_declarator())->what_declarator())->what_function())->what_var();
            std::string type = ((Declaration_Specifiers *) decl_node->what_decl_specifier())->what_decl_specifiers();
            if (symbol_table->find(function) == "1") {
                symbol_table->modify(function, "function", type);
            }
        }
    }
    else if (node->what_type() == "Init_Declarator_List") {
        init_decl_list_node = (Init_Declarator_List *) node;                                        count++;
        traverse_AST_print(dst, asmt, init_decl_list_node->what_init_declarator_list(), symbol_table);    count++;
        traverse_AST_print(dst, asmt, init_decl_list_node->what_init_declarator(), symbol_table);         count--;
    }
    else if (node->what_type() == "Declaration_Specifiers") {
        decl_specifiers_node = (Declaration_Specifiers *) node;
        int j = count + 1;
        while(j--) { dst << "\t"; }
        dst << "|";
        dst << decl_specifiers_node->what_decl_specifiers() << std::endl;
        count--;
    }
    else if (node->what_type() == "Declarator") {
        declarator_node = (Declarator *) node;                                      count++;
        traverse_AST_print(dst, asmt, declarator_node->what_declarator(), symbol_table);  count++;
        traverse_AST_print(dst, asmt, declarator_node->what_initializer(), symbol_table); count--;
        if (declarator_node->what_declarator()->what_type() != "Function_Declaration") {
            std::string variable = ((Variable *) declarator_node->what_declarator())->what_var();
            if (declarator_node->what_initializer() != nullptr) {
                std::string value_type = (declarator_node->what_initializer())->what_type();
                if (value_type == "Int_Constant") {
                    std::string value = std::to_string(((Int_Constant *) declarator_node->what_initializer())->what_val());
                    if (symbol_table->find(variable) == "1") {
                        symbol_table->modify(variable, symbol_table->what_scope(variable), symbol_table->what_type(variable), value);
                    }
                    //asmt << "\t" << "li\t" << "$2," << value << std::endl;
                    //asmt << "\t" << "sw\t" << "$2," << (symbol_table->count_local_var()+1)*4 << "($fp)" << std::endl;
                    symbol_table->modify(variable, symbol_table->what_scope(variable), symbol_table->what_type(variable), symbol_table->what_value(variable), symbol_table->count_local_var()*4+4);
                }
                else if (value_type == "Float_Constant") {
                    std::string value = std::to_string(((Float_Constant *) declarator_node->what_initializer())->what_val());
                    if (symbol_table->find(variable) == "1") {
                        symbol_table->modify(variable, symbol_table->what_scope(variable), symbol_table->what_type(variable), value);
                    }
                }
                else if (value_type == "Char_Constant") {
                    std::string value = (std::string) (((Char_Constant *) declarator_node->what_initializer())->what_val());
                    if (symbol_table->find(variable) == "1") {
                        symbol_table->modify(variable, symbol_table->what_scope(variable), symbol_table->what_type(variable), value);
                    }
                }
            }
        }
    }
    else if (node->what_type() == "Postfix_Expression") { 
        postfix_expr_node = (Postfix_Expression *) node;
        count++;
        traverse_AST_print(dst, asmt, postfix_expr_node->what_postfix_expr(), symbol_table);
        int j = count + 1;
        while(j--) { dst << "\t"; }
        dst << "|";
        dst << postfix_expr_node->what_op() << std::endl;
        count--;
        j = count + 1;
        while(j--) { dst << "\t"; }
        dst << "|";
        dst << postfix_expr_node->what_name() << std::endl;
        count--;
    }    
    else if (node->what_type() == "Conditional_Expression") { 
        cond_expr_node = (Conditional_Expression *) node;
        count++;
        traverse_AST_print(dst, asmt, cond_expr_node->what_cond(), symbol_table);
        count++;
        traverse_AST_print(dst, asmt, cond_expr_node->what_expr_true(), symbol_table); 
        count++;
        traverse_AST_print(dst, asmt, cond_expr_node->what_expr_false(), symbol_table);  
        count = 0;
    }    
    else if (node->what_type() == "Assignment_Expression") { 
        assign_expr_node = (Assignment_Expression *) node; count++;
        traverse_AST_print(dst, asmt, assign_expr_node->what_var(), symbol_table); count++;
        int j = count + 1;
        while(j--) { dst << "\t"; }
        dst << "|";
        dst << assign_expr_node->what_op() << std::endl;
        traverse_AST_print(dst, asmt, assign_expr_node->what_val(), symbol_table);
        count--;
    }    
    else if (node->what_type() == "Empty_Expression") { 
        ; 
    }    
    else if (node->what_type() == "And_Expression") { 
        and_expr_node = (And_Expression *) node;
        traverse_AST_print(dst, asmt, and_expr_node->what_left(), symbol_table);
        traverse_AST_print(dst, asmt, and_expr_node->what_right(), symbol_table);
    }    
    else if (node->what_type() == "Exclusive_Or_Expression") { 
        excl_or_expr_node = (Exclusive_Or_Expression *) node;
        traverse_AST_print(dst, asmt, excl_or_expr_node->what_left(), symbol_table);
        traverse_AST_print(dst, asmt, excl_or_expr_node->what_right(), symbol_table);
    }    
    else if (node->what_type() == "Inclusive_Or_Expression") { 
        incl_or_expr_node = (Inclusive_Or_Expression *) node;
        traverse_AST_print(dst, asmt, incl_or_expr_node->what_left(), symbol_table);
        traverse_AST_print(dst, asmt, incl_or_expr_node->what_right(), symbol_table);
    }    
    else if (node->what_type() == "Logical_And_Expression") { 
        logic_and_expr_node = (Logical_And_Expression *) node;
        traverse_AST_print(dst, asmt, logic_and_expr_node->what_left(), symbol_table);
        traverse_AST_print(dst, asmt, logic_and_expr_node->what_right(), symbol_table);
    }    
    else if (node->what_type() == "Logical_Or_Expression") { 
        logic_or_expr_node = (Logical_Or_Expression *) node;
        traverse_AST_print(dst, asmt, logic_or_expr_node->what_left(), symbol_table);
        traverse_AST_print(dst, asmt, logic_or_expr_node->what_right(), symbol_table);
    }
    else if (node->what_type() == "Unary_Expression") { 
        unary_expr_node = (Unary_Expression *) node;
        int j = count + 1;
        while(j--) { dst << "\t"; }
        dst << "|" << unary_expr_node->what_op() << std::endl; 
        count--;
        traverse_AST_print(dst, asmt, unary_expr_node->what_unary_expr(), symbol_table);
    }     
    else if (node->what_type() == "Multiplicative_Expression") { 
        mult_expr_node = (Multiplicative_Expression *) node;
        count++;
        traverse_AST_print(dst, asmt, mult_expr_node->what_left(), symbol_table);
        int j = count + 1;
        while(j--) { dst << "\t"; }
        dst << "|";
        dst << mult_expr_node->what_op() << std::endl;
        traverse_AST_print(dst, asmt, mult_expr_node->what_right(), symbol_table);  
        count = 0;
    }    
    else if (node->what_type() == "Additive_Expression") { 
        add_expr_node = (Additive_Expression *) node;
        count++;
        traverse_AST_print(dst, asmt, add_expr_node->what_left(), symbol_table);
        int j = count + 1;
        while(j--) { dst << "\t"; }
        dst << add_expr_node->what_op() << std::endl;
        traverse_AST_print(dst, asmt, add_expr_node->what_right(), symbol_table);  
        count--;
        /*
        // addition
        if(add_expr_node->what_op() == "+") {
            if (add_expr_node->what_left()->what_type() == "Variable_Normal" && add_expr_node->what_right()->what_type() == "Int_Constant") {
                asmt << "addu\t" << "$2" <<
            }
        } // subtraction
        else {
            ;
        }
        */
    }    
    else if (node->what_type() == "Shift_Expression") { 
        shift_expr_node = (Shift_Expression *) node;
        count++;
        traverse_AST_print(dst, asmt, shift_expr_node->what_left(), symbol_table);
        int j = count + 1;
        while(j--) { dst << "\t"; }
        dst << shift_expr_node->what_op() << std::endl;
        traverse_AST_print(dst, asmt, shift_expr_node->what_right(), symbol_table);
        count = 0;   
    }    
    else if (node->what_type() == "Relational_Expression") { 
        relational_expr_node = (Relational_Expression *) node;
        count++;
        traverse_AST_print(dst, asmt, relational_expr_node->what_left(), symbol_table);
        int j = count + 1;
        while(j--) { dst << "\t"; }
        dst << relational_expr_node->what_op() << std::endl;
        traverse_AST_print(dst, asmt, relational_expr_node->what_right(), symbol_table);
        count = 0;   
    }    
    else if (node->what_type() == "Equality_Expression") { 
        equality_expr_node = (Equality_Expression *) node;
        count++;
        traverse_AST_print(dst, asmt, equality_expr_node->what_left(), symbol_table);
        int j = count + 1;
        while(j--) { dst << "\t"; }
        dst << equality_expr_node->what_op() << std::endl;
        traverse_AST_print(dst, asmt, equality_expr_node->what_right(), symbol_table);
        count = 0;  
    }   
    else if (node->what_type() == "Function_Declaration") { 
        func_decl_node = (Function_Declaration *) node;
        count++;
        traverse_AST_print(dst, asmt, func_decl_node->what_function(), symbol_table);
        count++;
        traverse_AST_print(dst, asmt, func_decl_node->what_params(), symbol_table);
        count = 0;
        std::string function = ((Variable *) func_decl_node->what_function())->what_var();
        if (symbol_table->find(function) == "1") {
            symbol_table->modify(function, "function");
        }
    }     
    else if (node->what_type() == "Function_Definition") { 
        func_def_node = (Function_Definition *) node;
        count++;
        traverse_AST_print(dst, asmt, func_def_node->what_type_specifier(), symbol_table);
        count++;
        traverse_AST_print(dst, asmt, func_def_node->what_function(), symbol_table);
        count++;
        traverse_AST_print(dst, asmt, func_def_node->what_params(), symbol_table);
        count++;
        traverse_AST_print(dst, asmt, func_def_node->what_definition(), symbol_table); 
        count = 0;
        std::string function = ((Variable *) ((Function_Declaration *) func_def_node->what_function())->what_function())->what_var();
        std::string type = ((Declaration_Specifiers *) func_def_node->what_type_specifier())->what_decl_specifiers();
        if (symbol_table->find(function) == "1") {
            symbol_table->modify(function, "function", type, std::to_string(depth));
        }
        depth=0;
    }    
    else if (node->what_type() == "Parameter_List") { 
        param_list_node = (Parameter_List *) node;              count++;
        traverse_AST_print(dst, asmt, param_list_node->what_param_list(), symbol_table);  count++;count++;
        traverse_AST_print(dst, asmt, param_list_node->what_param(), symbol_table);       count=0;
    }    
    else if (node->what_type() == "Parameter_Declaration") { 
        param_decl_node = (Parameter_Declaration *) node;
        count++;
        traverse_AST_print(dst, asmt, param_decl_node->what_decl_specifiers(), symbol_table);
        count++;
        traverse_AST_print(dst, asmt, param_decl_node->what_declarator(), symbol_table);
        count = 0;
        std::string variable = ((Variable *) param_decl_node->what_declarator())->what_var();
        std::string type = ((Declaration_Specifiers *) param_decl_node->what_decl_specifiers())->what_decl_specifiers();
        if (symbol_table->find(variable) == "1") {
            symbol_table->modify(variable, "parameter", type);
            symbol_table->find(variable);
        }
    }    
    else if (node->what_type() == "Function_Call") { 
        func_call_node = (Function_Call *) node;
        count++;
        traverse_AST_print(dst, asmt, func_call_node->what_declarator(), symbol_table);
        count++;
        traverse_AST_print(dst, asmt, func_call_node->what_args(), symbol_table);
        count = 0;
    }    
    else if (node->what_type() == "Argument_List") { 
        arg_list_node = (Argument_List *) node;
        count++;
        traverse_AST_print(dst, asmt, arg_list_node->what_arg_list(), symbol_table);
        count++;
        traverse_AST_print(dst, asmt, arg_list_node->what_arg(), symbol_table);
        count = 0;
    }
}


#endif 