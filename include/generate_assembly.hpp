#ifndef generate_assembly_hpp
#define generate_assembly_hpp

#include "ast.hpp"
#include "ast_traverse.hpp"
#include "compile_helper.hpp"
#include "allocate_register.hpp"
#include "context.hpp"

// FOR THE COMPILER TO FIND ALL FUNCTIONS
void compile_func_def(std::ostream &mips, const Function_Definition *func_def, Symbol_Table *symbol_table, Register &regs);
void compile_func_call(std::ostream &mips, const Function_Call *func_call, Symbol_Table *symbol_table, Register &regs, Context &context, std::string scope, const std::string &dst_reg); // TODO: FUNCTION CALL
void compile_compound_stmt(std::ostream &mips, const Compound_Statement *compound_stmt, Symbol_Table *symbol_table, Register &regs, Context &context, std::string scope);
void compile_decl_list(std::ostream &mips, const Declaration_List *decl_list, Symbol_Table *symbol_table, Register &regs, Context &context, std::string scope);
void compile_stmt_list(std::ostream &mips, const Statement_List *stmt_list, Symbol_Table *symbol_table, Register &regs, Context &context, std::string scope);
void compile_decl(std::ostream &mips, const Declaration *decl, Symbol_Table *symbol_table, Register &regs, Context &context, std::string scope);
void compile_stmt(std::ostream &mips, const Node *stmt, Symbol_Table *symbol_table, Register &regs, Context &context, std::string scope);
void store_reg(std::ostream &mips, const Variable *var, Symbol_Table *symbol_table, Register &regs, Context &context, std::string scope, const std::string &src_reg, bool is_decl);
void compile_maths_expr(std::ostream &mips, const Node *maths, Symbol_Table *symbol_table, Register &regs, Context &context, std::string scope, const std::string &dst_reg);
void load_reg(std::ostream &mips, const Variable *var, Symbol_Table *symbol_table, Register &regs, Context &context, std::string scope, const std::string &dst_reg);



void compile_if_stmt(std::ostream &mips, const If_Statement *if_stmt, Symbol_Table *symbol_table, Register &regs, Context &context, std::string scope);
void compile_for_stmt(std::ostream &mips, const For_Statement *for_stmt, Symbol_Table *symbol_table, Register &regs, Context &context, std::string scope);
void compile_while_stmt(std::ostream &mips, const While_Statement *while_stmt, Symbol_Table *symbol_table, Register &regs, Context &context, std::string scope);
void compile_return_stmt(std::ostream &mips, const Return_Statement *return_stmt, Symbol_Table *symbol_table, Register &regs, Context &context, std::string scope);
void compile_global_var_decl(std::ofstream& mips, const Declaration *decl);
void compile_break_stmt(std::ostream &mips, const Break_Statement *break_stmt, Symbol_Table *symbol_table, Register &regs, Context &context, std::string scope);
void compile_continue_stmt(std::ostream &mips, const Continue_Statement *continue_stmt, Symbol_Table *symbol_table, Register &regs, Context &context, std::string scope);
void compile_default_stmt(std::ostream &mips, const Default_Statement *default_stmt, Symbol_Table *symbol_table, Register &regs, Context &context, std::string scope, const std::string& end_switch_id);
void compile_switch_stmt(std::ostream &mips, const Switch_Statement *switch_stmt, Symbol_Table *symbol_table, Register &regs, Context &context, std::string scope);
void compile_case_stmt_list(std::ostream& mips, const Case_Statement_List* case_stmt_list, Symbol_Table *symbol_table, const std::string& test_reg, const Node* switch_test,  const std::string& def_reg,  bool& there_is_default, Context &context, Register &regs, const std::string& end_switch_id, std::string scope);
void compile_case_stmt(std::ostream &mips, const Case_Statement *case_stmt,  Symbol_Table *symbol_table, Register &regs, Context &context, std::string scope, const std::string& test_reg, const Node* switch_test, const std::string& def_reg);
void compile_enum_decl_list(std::ostream& mips, const Enum_Declaration_List* enum_decl_list,int number_prev);
void compile_enum_decl(std::ostream& mips, const Enum_Declaration* enum_decl, int& number_prev);

Global_Var global_var;
Function_Decl func_decl;
std::vector<std::string> return_labels;

// ---------

void c_to_mips(std::ofstream &mips, std::vector<const Node *> &roots, Symbol_Table *symbol_table, Register &regs) {
    // Any Declarations
    for (const Node *ast : roots) {
        if(ast->what_type() == "Declaration") {
            const Declaration *decl = dynamic_cast<const Declaration *>(ast);
            const Declarator *declarator = dynamic_cast<const Declarator *>(dynamic_cast<const Init_Declarator_List *>(decl->what_init_declarator_list())->what_init_declarator());
            if (declarator->what_declarator()->what_type() == "Function_Declaration") {
                // TODO: Function Declaration
                const std::string &func_name = dynamic_cast<const Variable *>(dynamic_cast<const Function_Declaration *>(declarator->what_declarator())->what_function())->what_var();
                func_decl.add_id(func_name);
            }
            else { compile_global_var_decl(mips, decl); }
        }
        if(ast->what_type() == "Enum_Declaration_List") {
            const Enum_Declaration_List* enum_decl_list = dynamic_cast<const Enum_Declaration_List*>(ast);
            compile_enum_decl_list(mips, enum_decl_list, -1);
        }
    }
    mips << std::endl << std::endl;
    // Function Definition
    for (const Node *ast : roots) {
        if(ast->what_type() == "Function_Definition") {
            const Function_Definition *func_def = dynamic_cast<const Function_Definition *> (ast);
            compile_func_def(mips, func_def, symbol_table, regs);
        }
    }
}

void compile_global_var_decl(std::ofstream& mips, const Declaration *decl) {
    const std::string& type = dynamic_cast<const Declaration_Specifiers *>(decl->what_decl_specifier())->what_decl_specifiers();
    const Declarator *declarator = dynamic_cast<const Declarator *>(dynamic_cast<const Init_Declarator_List *>(decl->what_init_declarator_list())->what_init_declarator());
    const Variable* variable = dynamic_cast<const Variable*>(declarator->what_declarator());
    const std::string& var_type = variable->what_type();
    const std::string& var_name = variable->what_var();
    global_var.add_global_var(var_name, var_type);
    if (type == "int") {
        if (var_type == "Variable_Normal") {
            if (declarator->what_initializer() != nullptr) {
                int init = Helper::interpret_constant_expr(declarator->what_initializer());
                mips << var_name << ":" << "\t" << ".word" << "\t" << init << std::endl;
            } 
            else {
                // TODO: Non-initialized global variable that could be assigned to some value later
            }
        } 
        else if (var_type == "Variable_Array") {
            int size_in_bytes = Helper::interpret_constant_expr(variable->what_index_array());
            mips << var_name << ":" << "\t" << ".space" << "\t" << size_in_bytes << std::endl;
        } 
        else if (var_type == "Variable_Pointer") {
            if (declarator->what_initializer()) {
                int init = Helper::interpret_constant_expr(declarator->what_initializer());
                mips << var_name << ":" << "\t" << ".word" << "\t" << init << std::endl;
            }
            else {
                // TODO: Non-initialized global variable that could be assigned to some value later
            }
        }
    }
}

void compile_func_def(std::ostream &mips, const Function_Definition *func_def, Symbol_Table *symbol_table, Register &regs) {
    std::string func_type = func_def->what_type_specifier()->what_type();
    std::string func_name = dynamic_cast<const Variable *>(dynamic_cast<const Function_Declaration *>(func_def->what_function())->what_function())->what_var();
    const Parameter_List *param_list = dynamic_cast<const Parameter_List *>(dynamic_cast<const Function_Declaration *>(func_def->what_function())->what_params());
    const Compound_Statement *def = dynamic_cast<const Compound_Statement *>(func_def->what_definition());

    int depth = std::stoi(symbol_table->what_value(func_name));
    int stack_frame = 0;
    if (depth == 0) { stack_frame = 8; }
    else {
        stack_frame = 16;
        while(depth>0) {
            stack_frame += 8;
            depth-=2; 
        }
    }
    stack_frame = Helper::what_stack_frame_size(func_def);
    std::cerr << "STACK FRAME: " << stack_frame << std::endl;
    const std::string& epilogue_label = Helper::generate_unique_id(func_name+"_epilogue");
    Context context(stack_frame, epilogue_label);

    mips << "\t" << ".align" << "\t" << "2" << std::endl;
    mips << "\t" << ".globl" << "\t" << func_name << std::endl;
    mips << "\t" << ".set"  << "\t" << "nomips16" << std::endl;
    mips << "\t" << ".set"  << "\t" << "nomicromips" << std::endl;
    mips << "\t" << ".ent"  << "\t" << func_name << std::endl;
    mips << "\t" << ".type" << "\t" << func_name << "," << " " << "@function" << std::endl;

    if (param_list == nullptr) {
        mips << func_name << ":"  << std::endl;
        mips << "\t" << "addiu" << "\t" << "$sp,$sp," << -stack_frame  << std::endl;
        mips << "\t" << "sw" << "\t\t" << "$31," << stack_frame-4 << "($sp)" << std::endl;
        mips << "\t" << "sw" << "\t\t" << "$fp," << stack_frame-8 << "($sp)" << std::endl;
        mips << "\t" << "move" << "\t" << "$fp,$sp" << std::endl << std::endl;

        std::string func_scope = Helper::generate_unique_id(func_name);
        context.add_scope(func_scope);
        std::cerr << "[compile_func_def]: compound_stmt" << std::endl;
        compile_compound_stmt(mips, def, symbol_table, regs, context, func_scope);

        for(std::string return_id : return_labels) {
            mips << return_id << ":" << std::endl;
        }
        return_labels.clear();
        mips << std::endl << "\t" << "move\t" << "$sp,$fp" << std::endl;
        mips << "\t" << "lw" << "\t\t"  << "$31," << stack_frame-4 << "($sp)" << std::endl;
        mips << "\t" << "lw" << "\t\t"  << "$fp," << stack_frame-8 << "($sp)" << std::endl;
        mips << "\t" << "addiu\t" << "$sp,$sp," << stack_frame << std::endl;
        mips << "\t" << "j"  << "\t\t" << "$31" << std::endl;
        mips << "\t" << "nop" << std::endl;
    }
    else {
        // TODO: FUNCTIONS WITH ARGUMENTS NEED TO STORE THE ARGUMENTS IN REGISTERS -> DIFF ASM PATTERN
        std::cerr << "[compile_func_dec]: parameter list" << std::endl;
        // Prologue
        // Function Assembly
        // Epilogue

        mips << func_name << ":"  << std::endl;
        mips << "\t" << "addiu" << "\t" << "$sp,$sp," << -stack_frame  << std::endl;
        mips << "\t" << "sw" << "\t\t" << "$fp," << stack_frame-4 << "($sp)" << std::endl;
        mips << "\t" << "move" << "\t" << "$fp,$sp" << std::endl << std::endl;

        std::string func_scope = Helper::generate_unique_id(func_name);
        context.add_scope(func_scope);

        std::vector<std::string> param_names;
        Helper::extract_param_names(param_list, param_names);
        std::cerr << "[compile_func_dec]: parameter list: " << param_names.size() << std::endl;
        if (func_name != "main") {
            for(unsigned i = 0; i<param_names.size(); i++) {
                mips << "\t" << "sw" << "\t\t" << "$a" << i << ", " << i*4 + stack_frame << "($sp)" << std::endl;
                context.save_param_offset(param_names.at(i), i*4 + stack_frame, func_scope);
            }
        }
        compile_compound_stmt(mips, def, symbol_table, regs, context, func_scope);
        if (func_name != "main") {
            for(unsigned i = 0; i<param_names.size(); i++) {
                mips << "\t" << "lw" << "\t\t" << "$a" << i << ", " << i*4 + stack_frame << "($fp)" << std::endl;
            }
        }
        std::cerr << "[compile_func_def]: compound_stmt" << std::endl;

        for(std::string return_id : return_labels) {
            mips << return_id << ":" << std::endl;
        }
        return_labels.clear();
        mips << std::endl << "\t" << "move\t" << "$sp,$fp" << std::endl;
        mips << "\t" << "lw" << "\t\t"  << "$fp," << stack_frame-4 << "($sp)" << std::endl;
        mips << "\t" << "addiu\t" << "$sp,$sp," << stack_frame << std::endl;
        mips << "\t" << "j"  << "\t\t" << "$31" << std::endl;
        mips << "\t" << "nop" << std::endl;
    }

    context.remove_scope();

    mips << "\t" << ".set"  << "\t" << "macro"   << std::endl;
    mips << "\t" << ".set"  << "\t" << "reorder" << std::endl;
    mips << "\t" << ".end"  << "\t" << func_name << std::endl;
    mips << "\t" << ".size" << "\t" << func_name << ", .-" << func_name << std::endl << std::endl << std::endl;
}

// TODO: SEG FAULT HERE FIX AS THE SAME USUAL LIST STUFF
void compile_func_call_arg_list(std::ostream &mips, const Argument_List *arg_list, Symbol_Table *symbol_table,  Register &regs, Context &context, const std::string &scope, int num, std::vector<std::string> &arg_regs) {
    std::string arg_reg = regs.use_available_register();
    arg_regs.push_back(arg_reg);
    if(arg_list->what_arg_list() == nullptr) {
        const Node *arg = arg_list->what_arg();
        compile_maths_expr(mips, arg, symbol_table, regs, context, scope, arg_reg);
    }
    else if(arg_list->what_arg_list() != nullptr) {
        const Node *arg = arg_list->what_arg();
        const Argument_List *arg_list_list = dynamic_cast<const Argument_List *>(arg_list->what_arg_list());
        compile_func_call_arg_list(mips, arg_list_list, symbol_table, regs, context, scope, num+1, arg_regs);
        compile_maths_expr(mips, arg, symbol_table, regs, context, scope, arg_reg);
    }
}

void compile_func_call(std::ostream &mips, const Function_Call *func_call, Symbol_Table *symbol_table, Register &regs, Context &context, std::string scope, const std::string &dst_reg) {
    const std::string& func_name = dynamic_cast<const Variable *>(func_call->what_declarator())->what_var();
    const Argument_List * arg_list = dynamic_cast<const Argument_List *>(func_call->what_args());
    std::cerr << "[compile_func_call] : before anything" << std::endl;
    if (func_decl.is_id_no_def(func_name)) {
        mips << "\t" << "lui" << "\t\t" << "$28, %hi(__gnu_local_gp)" << std::endl;
        mips << "\t" << "addiu" << "\t" << "$28, $28, %lo(__gnu_local_gp)" << std::endl;
    }
    const std::vector<std::string>& unavailable_regs = regs.get_unavailable_registers();
    std::cerr << "[compile_func_call] : after is id no def" << std::endl;
    std::string arg_scope = Helper::generate_unique_id("arg_scope");
    context.add_scope(arg_scope);
   // for (const std::string& reg : unavailable_regs) {
   //         int offset = context.store_in_stack(reg, arg_scope, true);
   //         mips << "\t" << "sw" << "\t\t" << reg << ", " << offset << "($fp)" << std::endl;
   // }
    if (arg_list != nullptr) {
        std::cerr << "[compile_func_call] : after sth" << std::endl;
        std::vector<std::string> arg_regs; 
        compile_func_call_arg_list(mips, arg_list, symbol_table, regs, context, scope, 0, arg_regs);
        for (unsigned int i = 0; i < arg_regs.size(); i++) {
            mips << "\t" << "move\t " << "$a" << i << ", " << arg_regs[i] << std::endl;
            regs.empty_used_register(arg_regs[i]);
        }
    }

    if (func_decl.is_id_no_def(func_name)) {
        const std::string tmp_reg = regs.use_available_register();
        mips << "\t" << "lw" << "\t\t" << tmp_reg << ", %call16(" << func_name << ")($28)" << std::endl;
        mips << "\t" << "move" << "\t" << "$25," << tmp_reg << std::endl;
        regs.empty_used_register(tmp_reg);
        mips << "\t" << ".reloc 1f,R_MIPS_JALR," << func_name << std::endl;
        mips << "1:   jalr" << "\t"  << "$25" << std::endl;
        mips << "\t" << "nop" << std::endl;
    } 
    else {
        mips << "\t" << "jal" << "\t\t" << func_name << std::endl;
        mips << "\t" << "nop" << std::endl;
    }
    //for (const std::string& reg : unavailable_regs) {
    //    int offset = context.what_var_offset(reg);
    //    mips << "\t" << "lw" << "\t\t" << reg << "," << offset << "($fp)" << std::endl;
    //}
    context.remove_scope();
    mips << "\t" << "move" << "\t" << dst_reg << ",$2" << std::endl;
}

void compile_compound_stmt(std::ostream &mips, const Compound_Statement *compound_stmt, Symbol_Table *symbol_table, Register &regs, Context &context, std::string scope) {
    const Declaration_List *decl_list = dynamic_cast<const Declaration_List *>(compound_stmt->what_decl_list());
    const Statement_List *stmt_list = dynamic_cast<const Statement_List *>(compound_stmt->what_stmt_list());
    if (decl_list != nullptr) {
        std::cerr << "[compile_compound_stmt]: decl_list" << std::endl;
        compile_decl_list(mips, decl_list, symbol_table, regs, context, scope);
    }
    if (stmt_list != nullptr) {
        std::cerr << "[compile_compound_stmt]: stmt_list" << std::endl;
        compile_stmt_list(mips, stmt_list, symbol_table, regs, context, scope);
    }
}

void compile_decl_list(std::ostream &mips, const Declaration_List *decl_list, Symbol_Table *symbol_table, Register &regs, Context &context, std::string scope) {
    const Declaration_List *decl_list_list = dynamic_cast<const Declaration_List *>(decl_list->what_decl_list());
    if (decl_list_list == nullptr) {
        std::cerr << "[compile_decl_list]: decl_list_list = nullptr" << std::endl;
        const Declaration *decl = dynamic_cast<const Declaration *>(decl_list->what_decl());
        compile_decl(mips, decl, symbol_table, regs, context, scope);
    }
    else { // If there is decl_list_list there must be decl after that
        std::cerr << "[compile_decl_list]: decl_list_list != nullptr" << std::endl;
        const Declaration *decl = dynamic_cast<const Declaration *>(decl_list->what_decl());
        compile_decl_list(mips, decl_list_list, symbol_table, regs, context, scope);
        compile_decl(mips, decl, symbol_table, regs, context, scope);
    }
}

void compile_decl(std::ostream &mips, const Declaration *decl, Symbol_Table *symbol_table, Register &regs, Context &context, std::string scope) {
    const Declarator *declarator = dynamic_cast<const Declarator *> (dynamic_cast<const Init_Declarator_List *> (decl->what_init_declarator_list())->what_init_declarator());
    const Variable *var = dynamic_cast<const Variable *> (declarator->what_declarator());
    std::string var_name = var->what_var();

    if (var->what_type() == "Variable_Normal") {
        if (declarator->what_initializer() != nullptr) {
            std::string reg = regs.use_available_register();
            compile_maths_expr(mips, declarator->what_initializer(), symbol_table, regs, context, scope, reg);
            store_reg(mips, var, symbol_table, regs, context, scope, reg, true);
            regs.empty_used_register(reg);
        }
        else {
            store_reg(mips, var, symbol_table, regs, context, scope, "$0", true);
        }
    }
    else if (var->what_type() == "Variable_Array") {
        int size = Helper::interpret_constant_expr(var->what_index_array());
        context.reserve_array_space(var->what_type(), size, scope);
    }
    else if (var->what_type() == "Variable_Pointer") {
        int offset = context.store_in_stack(var->what_type(), scope, true);
        if (declarator->what_initializer() != nullptr) {
            std::string rhs_reg = regs.use_available_register();
            compile_maths_expr(mips, declarator->what_initializer(), symbol_table, regs, context, scope, rhs_reg);
            mips << "sw\t " << rhs_reg << "," << offset << "($fp)" <<  std::endl;
            regs.empty_used_register(rhs_reg);
        } else {
            mips << "sw\t " << "$0," << offset << "($fp)" <<  std::endl;
        }
    }
}

void compile_enum_decl_list(std::ostream& mips, const Enum_Declaration_List* enum_decl_list,int number_prev) {
    const Enum_Declaration* enum_decl = dynamic_cast<const Enum_Declaration*> (enum_decl_list->what_enum_decl());

    if (enum_decl_list->what_enum_decl_list()==nullptr) {
        // Last enum declaration.
        compile_enum_decl(mips, enum_decl, number_prev);
    } 
    else {
        const Enum_Declaration_List* next_enum_decl_list = dynamic_cast<const Enum_Declaration_List*> (enum_decl_list->what_enum_decl_list());
        compile_enum_decl_list(mips, next_enum_decl_list, number_prev);
        compile_enum_decl(mips, enum_decl, number_prev);
    }
}

void compile_enum_decl(std::ostream& mips, const Enum_Declaration* enum_decl, int& number_prev) {

    std::cerr << "ENUM SHIKI" << std::endl;
    const std::string& id = enum_decl->what_name();
    int val;
    if (enum_decl->what_val()!=nullptr) {
        val = Helper::interpret_constant_expr(enum_decl->what_val());
    } 
    else {
        val = number_prev + 1;
    }

    global_var.add_global_var(id, "enum");
    mips << id << ":" << "\t" << ".word" << " " << val << std::endl;

    number_prev = val;
}

void compile_stmt_list(std::ostream &mips, const Statement_List *stmt_list, Symbol_Table *symbol_table, Register &regs, Context &context, std::string scope) {
    const Statement_List *stmt_list_list = dynamic_cast<const Statement_List *>(stmt_list->what_stmt_list());
    if (stmt_list_list == nullptr) {
        std::cerr << "[compile_stmt_list]: stmt_list_list = nullptr" << std::endl;
        const Node *stmt = stmt_list->what_stmt(); // There is no node for statement -> expressions
        compile_stmt(mips, stmt, symbol_table, regs, context, scope);
    }
    else {
        std::cerr << "[compile_stmt_list]: stmt_list_list != nullptr" << std::endl;
        const Node *stmt = stmt_list->what_stmt(); // There is no node for statement -> expressions
        compile_stmt_list(mips, stmt_list_list, symbol_table, regs, context, scope);
        compile_stmt(mips, stmt, symbol_table, regs, context, scope);
    }
}

void compile_stmt(std::ostream &mips, const Node *stmt, Symbol_Table *symbol_table, Register &regs, Context &context, std::string scope) {
    std::cerr << "[compile_stmt]" << std::endl;
    const std::string& type = stmt->what_type();
    std::cerr << "[compile_stmt]: type" << std::endl;
    std::cerr << stmt->what_type() << std::endl;
    if (stmt->what_type() == "If_Statement") {
        std::cerr << "[compile_stmt]: if" << std::endl;
        const If_Statement *if_stmt = dynamic_cast<const If_Statement *>(stmt);
        compile_if_stmt(mips, if_stmt, symbol_table, regs, context, scope);
    }
    else if (stmt->what_type() == "While_Statement") {
        std::cerr << "[compile_stmt]: while" << std::endl;
        const While_Statement *while_stmt = dynamic_cast<const While_Statement *>(stmt);
        compile_while_stmt(mips, while_stmt, symbol_table, regs, context, scope);
    }
    else if (stmt->what_type() == "For_Statement") {
        const For_Statement *for_stmt = dynamic_cast<const For_Statement *>(stmt);
        compile_for_stmt(mips, for_stmt, symbol_table, regs, context, scope);
    }
    else if (stmt->what_type() == "Return_Statement") {
        std::cerr << "[compile_stmt]: return" << std::endl;
        const Return_Statement *return_stmt = dynamic_cast<const Return_Statement *>(stmt);
        compile_return_stmt(mips, return_stmt, symbol_table, regs, context, scope);
    }
    else if (stmt->what_type() == "Compound_Statement") {
        std::string compound_scope = Helper::generate_unique_id("compound_scope");
        context.add_scope(compound_scope);
        const Compound_Statement *compound_stmt = dynamic_cast<const Compound_Statement *>(stmt);
        compile_compound_stmt(mips, compound_stmt, symbol_table, regs, context, compound_scope);
        context.remove_scope();
    } // TODO: ADD FLOAT_CONSTANT , ETC ...
    else if (stmt->what_type() == "Variable" || stmt->what_type() == "Int_Constant" || stmt->what_type() == "Unary_Expression" || stmt->what_type() == "Postfix_Expression" 
          || stmt->what_type() == "Additive_Expression" || stmt->what_type() == "Multiplicative_Expression" || stmt->what_type() == "Shift_Expression" 
          || stmt->what_type() == "Relational_Expression" || stmt->what_type() == "Equality_Expression" || stmt->what_type() == "And_Expression"
          || stmt->what_type() == "Exclusive_Or_Expression" || stmt->what_type() == "Inclusive_Or_Expression" || stmt->what_type() == "Logical_And_Expression"
          || stmt->what_type() == "Logical_Or_Expression" || stmt->what_type() == "Conditional_Expression" || stmt->what_type() == "Assignment_Expression" || stmt->what_type() == "Function_Call") {
        std::string reg = regs.use_available_register();
        compile_maths_expr(mips, stmt, symbol_table, regs, context, scope, reg);
        regs.empty_used_register(reg);
    }
    else if (stmt->what_type() == "Break_Statement") {
        std::cerr << "[compile_stmt]: break" << std::endl;
        const Break_Statement* break_stmt = dynamic_cast<const Break_Statement*>(stmt); 
        compile_break_stmt(mips, break_stmt, symbol_table, regs, context, scope);
    }
    else if (stmt->what_type() == "Continue_Statement") {
        std::cerr << "[compile_stmt]: continue" << std::endl;
        const Continue_Statement* continue_stmt = dynamic_cast<const Continue_Statement*>(stmt); 
        compile_continue_stmt(mips, continue_stmt, symbol_table, regs, context, scope);
    }
    else if (stmt->what_type() == "Switch_Statement") {
        std::cerr << "[compile_stmt]: switch" << std::endl;
        const Switch_Statement* switch_stmt = dynamic_cast<const Switch_Statement*>(stmt); 
        compile_switch_stmt(mips, switch_stmt, symbol_table, regs, context, scope);
    }
    else if (stmt->what_type() == "Declaration_List") { // TODO: NOT SURE ABOUT THIS ONE

    }
}

// TODO IF, WHILE, FOR, RETURN, ASSIGN
void compile_if_stmt(std::ostream &mips, const If_Statement *if_stmt, Symbol_Table *symbol_table, Register &regs, Context &context, std::string scope) {
    std::string cond_reg = regs.use_available_register();
    compile_maths_expr(mips, if_stmt->what_cond(), symbol_table, regs, context, scope, cond_reg);
    std::string top_else_id = Helper::generate_unique_id("ELSE_TOP");
    mips << "\t" << "beq" << "\t\t" << cond_reg << "," << "$0" << "," << top_else_id << std::endl;
    mips << "\t" << "nop" << std::endl;
    regs.empty_used_register(cond_reg);
    std::string if_scope = Helper::generate_unique_id("if_scope");
    context.add_scope(if_scope);
    if (if_stmt->what_stmt_if()->what_type() == "Compound_Statement") {
        const Compound_Statement *body = dynamic_cast<const Compound_Statement *>(if_stmt->what_stmt_if());
        compile_compound_stmt(mips, body, symbol_table, regs, context, if_scope);
    }
    else {
        compile_stmt(mips, if_stmt->what_stmt_if(), symbol_table, regs, context, if_scope);
    }
    context.remove_scope();

    std::string end_if_id = Helper::generate_unique_id("IF_END");
    mips << "\t" << "b" << "\t\t" << end_if_id << std::endl;
    mips << "\t" << "nop" << std::endl;
    mips << top_else_id << ":" << std::endl;

    std::string else_scope = Helper::generate_unique_id("else_scope");
    context.add_scope(else_scope);
    if (if_stmt->what_stmt_else() != nullptr) {
        if (if_stmt->what_stmt_else()->what_type() == "Compound_Statement") {
            const Compound_Statement* body = dynamic_cast<const Compound_Statement*>(if_stmt->what_stmt_else());
            compile_compound_stmt(mips, body, symbol_table, regs, context, else_scope);
        } 
        else {
            compile_stmt(mips, if_stmt->what_stmt_else(), symbol_table, regs, context, else_scope);
        }
    }
    context.remove_scope();
    mips << end_if_id << ":" << std::endl;
}

void compile_while_stmt(std::ostream &mips, const While_Statement *while_stmt, Symbol_Table *symbol_table, Register &regs, Context &context, std::string scope) {
    std::string top_while_id = Helper::generate_unique_id("WHILE_TOP");
    mips <<top_while_id << ":" << std::endl;

    std::string cond_reg = regs.use_available_register();
    compile_maths_expr(mips, while_stmt->what_cond(), symbol_table, regs, context, scope, cond_reg);

    std::string end_while_id = Helper::generate_unique_id("WHILE_END");

    mips << "\t" << "beq" << "\t\t" << cond_reg << ",$0," << end_while_id << std::endl;
    mips << "\t" << "nop" << std::endl;

    regs.empty_used_register(cond_reg);
    std::string while_scope = Helper::generate_unique_id("while_scope");
    context.add_scope(while_scope);
    if (while_stmt->what_stmt()->what_type() == "Compound_Statement") {
        const Compound_Statement* body = dynamic_cast<const Compound_Statement*>(while_stmt->what_stmt());
        compile_compound_stmt(mips, body, symbol_table, regs, context, while_scope);
    } 
    else {
        compile_stmt(mips, while_stmt->what_stmt(), symbol_table, regs, context, while_scope);
    }

    mips << "\t" << "b" << "\t\t" << top_while_id << std::endl;
    mips << "\t" << "nop" << std::endl;
    mips << end_while_id << ":" << std::endl;

    context.remove_scope();
}

void compile_for_stmt(std::ostream &mips, const For_Statement *for_stmt, Symbol_Table *symbol_table, Register &regs, Context &context, std::string scope){
    std::string for_scope = Helper::generate_unique_id("for_scope");
    context.add_scope(for_scope);

    std::string top_for_id = Helper::generate_unique_id("FOR_TOP");
    mips <<top_for_id << ":" << std::endl;

    std::string top_increment_id = Helper::generate_unique_id("top_increment");

    std::string end_for_id = Helper::generate_unique_id("FOR_END");

    if (for_stmt->what_cond()->what_type() != "Empty_Expression"){
        std::string cond_reg = regs.use_available_register();
        compile_maths_expr(mips, for_stmt->what_cond(), symbol_table, regs, context, scope, cond_reg);
        mips << "\t" << "beq" << "\t\t" << cond_reg << ",$0," << end_for_id  << std::endl;
        mips << "\t" << "nop" << std::endl;

        regs.empty_used_register(cond_reg);
    }
    if (for_stmt->what_stmt()->what_type() == "Compound_Statement") {
        const Compound_Statement* body = dynamic_cast<const Compound_Statement*>(for_stmt->what_stmt());
        compile_compound_stmt(mips, body, symbol_table, regs, context, for_scope);
    } 
    else {
        compile_stmt(mips, for_stmt->what_stmt(), symbol_table, regs, context, for_scope);
    }

    mips << top_increment_id << ":" << std::endl;
    if ( for_stmt->what_inc() != nullptr){
        compile_stmt(mips, for_stmt->what_inc(), symbol_table, regs, context, for_scope);
    }

    mips << "\t" << "b" << "\t\t" << top_for_id << std::endl;
    mips << "\t" << "nop" << std::endl;
    mips << end_for_id << ":" << std::endl;

    context.remove_scope();
}

void compile_return_stmt(std::ostream &mips, const Return_Statement *return_stmt, Symbol_Table *symbol_table, Register &regs, Context &context, std::string scope) {
    if (return_stmt->what_expr() != nullptr) {
        std::string reg = regs.use_available_register();
        std::cerr << "[compile_return_stmt]: yep" << std::endl;
        compile_maths_expr(mips, return_stmt->what_expr(), symbol_table, regs, context, scope, reg);
        std::string return_id = Helper::generate_unique_id("return");
        return_labels.push_back(return_id);
        mips << "\t" << "move" << "\t" << "$2" << "," << reg << std::endl;
        mips << "\t" << "b" << "\t\t" << return_id << std::endl;
        mips << "\t" << "nop" << std::endl;
        regs.empty_used_register(reg);
    }
}

void compile_assign_expr(std::ostream &mips, const Assignment_Expression *assign_expr, Symbol_Table *symbol_table, Register &regs, Context &context, std::string scope, const std::string &dst_reg) {
    const Variable *var = dynamic_cast<const Variable *>(assign_expr->what_var()); 
    std::string reg = regs.use_available_register();
    compile_maths_expr(mips, assign_expr->what_val(), symbol_table, regs, context, scope, reg);
    if (assign_expr->what_op() == "="){
        mips << "\t" << "move" << "\t" << dst_reg << "," << reg << std::endl;
        store_reg(mips, var, symbol_table, regs, context, scope, dst_reg, false);    
    }
    else if (assign_expr->what_op() == "*="){
        load_reg(mips, var, symbol_table, regs, context, scope, dst_reg);
        mips << "\t" << "mult" << "\t" << dst_reg << "," << reg << std::endl;
        mips << "\t" << "mflo" << "\t" << dst_reg << std::endl;
        mips << "\t" << "nop" << std::endl;
        store_reg(mips, var, symbol_table, regs, context, scope, dst_reg, false);    
    }
    else if (assign_expr->what_op() == "/="){
        load_reg(mips, var, symbol_table, regs, context, scope, dst_reg);
        mips << "\t" << "div" << "\t" << dst_reg << "," << reg << std::endl;
        mips << "\t" << "mflo" << "\t" << dst_reg << std::endl;
        mips << "\t" << "nop" << std::endl;
        store_reg(mips, var, symbol_table, regs, context, scope, dst_reg, false);    
    }
    else if (assign_expr->what_op() == "%="){
        load_reg(mips, var, symbol_table, regs, context, scope, dst_reg);
        mips << "\t" << "mult" << "\t" << dst_reg << "," << reg << std::endl;
        mips << "\t" << "mfhi" << "\t" << dst_reg << std::endl;
        mips << "\t" << "nop" << std::endl;
        store_reg(mips, var, symbol_table, regs, context, scope, dst_reg, false);    
    }
    else if (assign_expr->what_op() == "+="){
        load_reg(mips, var, symbol_table, regs, context, scope, dst_reg);
        mips << "\t" << "addu" << "\t\t" << dst_reg << "," << reg << std::endl;
        store_reg(mips, var, symbol_table, regs, context, scope, dst_reg, false);    
    }
    else if (assign_expr->what_op() == "-="){
        load_reg(mips, var, symbol_table, regs, context, scope, dst_reg);
        mips << "\t" << "subu" << "\t\t" << dst_reg << "," << reg << std::endl;
        store_reg(mips, var, symbol_table, regs, context, scope, dst_reg, false);    
    }
    else if (assign_expr->what_op() == "<<="){
        load_reg(mips, var, symbol_table, regs, context, scope, dst_reg);
        mips << "\t" << "sllv" << "\t\t" << dst_reg << "," << reg << std::endl;
        store_reg(mips, var, symbol_table, regs, context, scope, dst_reg, false);    
    }
    else if (assign_expr->what_op() == ">>="){
        load_reg(mips, var, symbol_table, regs, context, scope, dst_reg);
        mips << "\t" << "srlv" << "\t\t" << dst_reg << "," << reg << std::endl;
        store_reg(mips, var, symbol_table, regs, context, scope, dst_reg, false);    
    }
    else if (assign_expr->what_op() == "&="){
        load_reg(mips, var, symbol_table, regs, context, scope, dst_reg);
        mips << "\t" << "and" << "\t\t" << dst_reg << "," << reg << std::endl;
        store_reg(mips, var, symbol_table, regs, context, scope, dst_reg, false);    
    }
    else if (assign_expr->what_op() == "^="){
        load_reg(mips, var, symbol_table, regs, context, scope, dst_reg);
        mips << "\t" << "xor" << "\t\t" << dst_reg << "," << reg << std::endl;
        store_reg(mips, var, symbol_table, regs, context, scope, dst_reg, false);    
    }
    else if (assign_expr->what_op() == "!="){
        load_reg(mips, var, symbol_table, regs, context, scope, dst_reg);
        mips << "\t" << "or" << "\t\t" << dst_reg << "," << reg << std::endl;
        store_reg(mips, var, symbol_table, regs, context, scope, dst_reg, false);    
    }
    regs.empty_used_register(reg);
}

    std::string top_default_id = Helper::generate_unique_id("DEFAULT_TOP");
    std::string end_default_id = Helper::generate_unique_id("DEFAULT_END");

// TODO: SWITCH CASE DEFAULT CONTINUE BREAK STMT
void compile_switch_stmt(std::ostream &mips, const Switch_Statement *switch_stmt, Symbol_Table *symbol_table, Register &regs, Context &context, std::string scope){
    bool there_is_default = false;
    std::string test_reg = regs.use_available_register();
    std::string end_switch_id = Helper::generate_unique_id("SWITCH_END");
    context.add_switch(top_default_id,end_switch_id);

    std::string def_reg = regs.use_available_register();
    mips << "\t" << "li" << "\t\t" << def_reg << ",1" << std::endl;

    if (switch_stmt->what_stmt()) {
        const Case_Statement_List* case_stmt_list = dynamic_cast<const Case_Statement_List*>(switch_stmt->what_stmt());
        compile_case_stmt_list(mips, case_stmt_list, symbol_table, test_reg, switch_stmt->what_expr(), def_reg, there_is_default, context, regs, end_switch_id, scope);
    }
    if (there_is_default){
        mips << "\t" << "bne" << "\t\t" << def_reg << ",$0," << top_default_id << std::endl;
    }

    mips <<end_switch_id << ":" << std::endl;

    regs.empty_used_register(test_reg);
    regs.empty_used_register(def_reg);
    context.remove_switch();
}

void compile_case_stmt_list(std::ostream& mips, const Case_Statement_List* case_stmt_list, Symbol_Table *symbol_table, const std::string& test_reg, const Node* switch_test,  const std::string& def_reg,  bool& there_is_default, Context &context, Register &regs, const std::string& end_switch_id, std::string scope){
    if(case_stmt_list->what_stmt_list()==nullptr) {
        if(case_stmt_list->what_stmt()->what_type() == "Case_Statement"){
            const Case_Statement* case_stmt = dynamic_cast<const Case_Statement*>(case_stmt_list->what_stmt());
            compile_case_stmt(mips, case_stmt, symbol_table, regs, context, scope, test_reg, switch_test, def_reg);
        }
        else if(case_stmt_list->what_stmt()->what_type() == "Default_Statement"){
            there_is_default = true;
            const Default_Statement* default_stmt = dynamic_cast<const Default_Statement*>(case_stmt_list->what_stmt());
            compile_default_stmt(mips, default_stmt, symbol_table, regs, context, scope, end_switch_id);
        }
    }
    else if (case_stmt_list->what_stmt_list()!=nullptr) {
        const Case_Statement_List* next_case_statement = dynamic_cast<const Case_Statement_List*>(case_stmt_list->what_stmt_list());
        compile_case_stmt_list(mips, next_case_statement,symbol_table, test_reg, switch_test, def_reg, there_is_default, context, regs, end_switch_id, scope);
        if(case_stmt_list->what_stmt()->what_type() == "Case_Statement"){
            const Case_Statement* case_stmt = dynamic_cast<const Case_Statement*>(case_stmt_list->what_stmt());
            compile_case_stmt(mips, case_stmt, symbol_table, regs, context, scope, test_reg, switch_test, def_reg); 
        }
        else if(case_stmt_list->what_stmt()->what_type() == "Default_Statement"){
            there_is_default = true;
            const Default_Statement* default_stmt = dynamic_cast<const Default_Statement*>(case_stmt_list->what_stmt());
            compile_default_stmt(mips, default_stmt, symbol_table, regs, context, scope, end_switch_id);
        }
    }
}

void compile_case_stmt(std::ostream &mips, const Case_Statement *case_stmt,  Symbol_Table *symbol_table, Register &regs, Context &context, std::string scope, const std::string& test_reg, const Node* switch_test, const std::string& def_reg) {
    std::string case_exp_reg = regs.use_available_register();
    std::string end_case_id = Helper::generate_unique_id("end_case");
 
    compile_maths_expr(mips, switch_test, symbol_table, regs, context, scope, test_reg);

    compile_maths_expr(mips, case_stmt->what_expr(), symbol_table, regs, context, scope, case_exp_reg);
    if (case_stmt->what_stmt_list()!=nullptr) { 
        traverse_AST_print(std::cerr, std::cerr, case_stmt->what_stmt_list(), symbol_table);
    }
    if (is_break_prev == false) {
        mips << "\t" << "bne" << "\t\t" << test_reg << "," << case_exp_reg << "," << end_case_id << std::endl;
    }
    else if (is_break_curr == true && is_break_prev == true){
        if (is_default == true) {
            mips << "\t" << "b" << "\t\t" << top_default_id << " # this" << std::endl;
        }
        else {
            mips << "\t" << "b" << "\t\t" << end_default_id << " # that" << std::endl;
            is_break_curr = false;
        }
    }
    mips << "\t" << "move" << "\t" << def_reg << ",$0" << std::endl;

    regs.empty_used_register(case_exp_reg);

    if (case_stmt->what_stmt_list()!=nullptr) {
        const Statement_List* stmt_list = dynamic_cast<const Statement_List*>(case_stmt->what_stmt_list());
        compile_stmt_list(mips, stmt_list,  symbol_table, regs, context ,scope);
    }
    mips << end_case_id << ":" << std::endl;
}

void compile_default_stmt(std::ostream &mips, const Default_Statement *default_stmt, Symbol_Table *symbol_table, Register &regs, Context &context, std::string scope, const std::string& end_switch_id){
    mips << "\t" << "b" << "\t\t" << end_default_id << std::endl;
    std::string top_default_id = context.what_default_label();
    mips << top_default_id <<":" << std::endl;


    if (default_stmt->what_stmt_list() != nullptr){
        const Statement_List* stmt_list = dynamic_cast<const Statement_List*>(default_stmt->what_stmt_list());
        compile_stmt_list(mips, stmt_list, symbol_table, regs, context ,scope);
    }

    mips << "\t" << "b" << "\t\t" << end_switch_id << std::endl;
    mips << end_default_id << ":" << std::endl;
}

void compile_continue_stmt(std::ostream &mips, const Continue_Statement *continue_stmt, Symbol_Table *symbol_table, Register &regs, Context &context, std::string scope){
    mips << "\t" << "b" << "\t\t" << context.what_continue_label()  << std::endl;
}

void compile_break_stmt(std::ostream &mips, const Break_Statement *break_stmt, Symbol_Table *symbol_table, Register &regs, Context &context, std::string scope){
    mips << "\t" << "b" << "\t\t" << context.what_break_label() << " # break" << std::endl;
}

void compile_maths_expr(std::ostream &mips, const Node *maths, Symbol_Table *symbol_table, Register &regs, Context &context, std::string scope, const std::string &dst_reg) {
    if (maths->what_type() == "Variable_Normal" || maths->what_type() == "Variable_Array" || maths->what_type() == "Variable_Pointer" ) {
        const Variable *variable = dynamic_cast<const Variable *>(maths);
        std::cerr << "!!!!!!!!!!!!!!!!!!!LOAD REGISTER" << std::endl;
        load_reg(mips, variable, symbol_table, regs, context, scope, dst_reg);
    }
    else if (maths->what_type() == "Int_Constant") {
        const Int_Constant *int_constant = dynamic_cast<const Int_Constant *>(maths);
        mips << "\t" << "li" << "\t\t" << dst_reg << "," << int_constant->what_val() << std::endl;
    }
    else if (maths->what_type() == "Float_Constant") {
        // TODO: ADD SUPPORT FOR FLOAT
    }
    else if (maths->what_type() == "Char_Constant") {
        const Char_Constant *character = dynamic_cast<const Char_Constant *>(maths);
        std::string str = character->what_val();
        char ch = str[1];
        std::cerr << "char_constant " << ch << "str: " << str << std::endl;
        int chint = ch;
        mips << "\t" << "li" << "\t\t" << dst_reg << "," << chint << std::endl;
    } 
    else if (maths->what_type() == "Str_Constant") {
        // TODO: ADD SUPPORT FOR STRINGS
    }
    else if (maths->what_type() == "Unary_Expression") {

        const Unary_Expression *unary_expr = dynamic_cast<const Unary_Expression *>(maths);
        std::string reg = regs.use_available_register();
        compile_maths_expr(mips, unary_expr->what_unary_expr(), symbol_table, regs, context, scope, reg);

        if (unary_expr->what_op() == "++") {
            const Variable *var = dynamic_cast<const Variable *>(unary_expr->what_unary_expr());
            std::string var_name = var->what_var();
            mips << "\t" << "addiu" << "\t" << dst_reg << "," << reg << ",1" << std::endl;
            store_reg(mips, var, symbol_table, regs, context, scope, dst_reg, false);
        }
        else if (unary_expr->what_op() == "--") {
            const Variable *var = dynamic_cast<const Variable *>(unary_expr->what_unary_expr());
            std::string var_name = var->what_var();
            mips << "\t" << "addiu" << "\t" << dst_reg << "," << reg << ",-1" << std::endl;
            store_reg(mips, var, symbol_table, regs, context, scope, dst_reg, false);
        }
        else if (unary_expr->what_op() == "-") { mips << "\t" << "subu" << "\t" << dst_reg << "," << "$0" << "," << reg << std::endl; }
        else if (unary_expr->what_op() == "~") { mips << "\t" << "not" << "\t" << dst_reg << "," << reg << std::endl; }
        else if (unary_expr->what_op() == "!") { mips << "\t" << "sltiu" << "\t" << dst_reg << "," << reg << "," << "1" << std::endl; }
        else if (unary_expr->what_op() == "&") {
            // TODO: DEREFERENCING OPERATOR
        }
        regs.empty_used_register(reg);
    }
    else if (maths->what_type() == "Postfix_Expression") {

        const Postfix_Expression *postfix_expr = dynamic_cast<const Postfix_Expression *>(maths);
        std::string reg = regs.use_available_register();
        compile_maths_expr(mips, postfix_expr->what_postfix_expr(), symbol_table, regs, context, scope, dst_reg);
        
        if (postfix_expr->what_op() == "++") {
            mips << "\t" << "addiu" << "\t" << dst_reg << ", " << dst_reg << ", 1" << std::endl;
            const Variable *var = dynamic_cast<const Variable *>(postfix_expr->what_postfix_expr());
            store_reg(mips, var, symbol_table, regs, context, scope, dst_reg, false);
        }
        else if (postfix_expr->what_op() == "--") {
            const Variable *var = dynamic_cast<const Variable *>(postfix_expr->what_postfix_expr());
            mips << "\t" << "addiu" << "\t" << dst_reg << ", " << dst_reg << ", -1" << std::endl;
            store_reg(mips, var, symbol_table, regs, context, scope, dst_reg, false);
        }

        regs.empty_used_register(reg);
    }
    else if (maths->what_type() == "Additive_Expression") {
        const Additive_Expression *add_expr = dynamic_cast<const Additive_Expression *>(maths);
        // int a = a + 4;
        compile_maths_expr(mips, add_expr->what_left(), symbol_table, regs, context, scope, dst_reg); // a
        std::string reg = regs.use_available_register(); // 4
        std::cerr << "SHOULD GO TO VARIABLE AND LOAD REGISTER: " + reg << std::endl;
        compile_maths_expr(mips, add_expr->what_right(), symbol_table, regs, context, scope, reg);
        if (add_expr->what_op() == "+") { mips << "\t" << "addu" << "\t" << dst_reg << "," << dst_reg << "," << reg << std::endl; /* a <- a + 4*/ }
        else if (add_expr->what_op() == "-") { mips << "\t" << "subu" << "\t" << dst_reg << "," << dst_reg << "," << reg << std::endl; }
        regs.empty_used_register(reg);
    }
    else if (maths->what_type() == "Multiplicative_Expression") {
        std::cerr << "[compile_maths_expr]: Multiplicative_Expression"<< std::endl;
        const Multiplicative_Expression *mult_expr = dynamic_cast<const Multiplicative_Expression *>(maths);
        compile_maths_expr(mips, mult_expr->what_left(), symbol_table, regs, context, scope, dst_reg);
        std::string reg = regs.use_available_register();
        compile_maths_expr(mips, mult_expr->what_right(), symbol_table, regs, context, scope, reg);
        std::cerr << mult_expr->what_op() << std::endl;
        if (mult_expr->what_op() == "*") {
            mips << "\t" << "mult" << "\t" << dst_reg << "," << reg << std::endl;
            mips << "\t" << "mflo" << "\t" << dst_reg << std::endl;
            mips << "\t" << "nop"  << std::endl;
            mips << "\t" << "nop"  << std::endl;
        }
        else if (mult_expr->what_op() == "/") { 
            mips << "\t" << "div"  << "\t" << dst_reg << "," << reg << std::endl;
            mips << "\t" << "mflo" << "\t" << dst_reg << std::endl;
            mips << "\t" << "nop"  << std::endl;
            mips << "\t" << "nop"  << std::endl;
        }
        else if (mult_expr->what_op() == "%") {
            mips << "\t" << "div"  << "\t" << dst_reg << "," << reg << std::endl;
            mips << "\t" << "mfhi" << "\t" << dst_reg << std::endl;
            mips << "\t" << "nop"  << std::endl;
            mips << "\t" << "nop"  << std::endl;
        }
        regs.empty_used_register(reg);
    }
    else if (maths->what_type() == "Shift_Expression") {
        const Shift_Expression *shift_expr = dynamic_cast<const Shift_Expression *>(maths);
        compile_maths_expr(mips, shift_expr->what_left(), symbol_table, regs, context, scope, dst_reg);
        std::string reg = regs.use_available_register();
        compile_maths_expr(mips, shift_expr->what_right(), symbol_table, regs, context, scope, reg);

        if (shift_expr->what_op() == "<<") { mips << "\t" << "sllv" << "\t" << dst_reg << "," << dst_reg << "," << reg << std::endl; }
        else if (shift_expr->what_op() == ">>") { mips << "\t" << "srlv" << "\t" << dst_reg << "," << dst_reg << "," << reg << std::endl; }
        regs.empty_used_register(reg);
    }
    else if (maths->what_type() == "Relational_Expression") {
        const Relational_Expression *relational_expr = dynamic_cast<const Relational_Expression *>(maths);
        compile_maths_expr(mips, relational_expr->what_left(), symbol_table, regs, context, scope, dst_reg);
        std::string reg = regs.use_available_register();
        compile_maths_expr(mips, relational_expr->what_right(), symbol_table, regs, context, scope, reg);
        if (relational_expr->what_op() == "<") { mips << "\t" << "slt" << "\t\t" << dst_reg << "," << dst_reg << "," << reg << std::endl; }
        else if (relational_expr->what_op() == ">") { mips << "\t" << "slt" << "\t\t" << dst_reg << "," << reg << "," << dst_reg << std::endl; }
        else if (relational_expr->what_op() == "<=") {
            mips << "\t" << "slt"  << "\t\t" << dst_reg << "," << reg << "," << dst_reg  << std::endl;
            mips << "\t" << "xori" << "\t" << dst_reg << "," << dst_reg << ",1" << std::endl;
        }
        else if (relational_expr->what_op() == ">="){ 
            mips << "\t" << "slt"  << "\t\t" << dst_reg << "," << dst_reg << "," << reg  << std::endl;
            mips << "\t" << "xori" << "\t" << dst_reg << "," << dst_reg << ",1" << std::endl;
        }
        regs.empty_used_register(reg);
    }
    else if (maths->what_type() == "Equality_Expression") {
        std::cerr << "[compile_maths_expr] : equality_expression" << std::endl;
        const Equality_Expression *equality_expr = dynamic_cast<const Equality_Expression *>(maths);
        compile_maths_expr(mips, equality_expr->what_left(), symbol_table, regs, context, scope, dst_reg);
        std::string reg = regs.use_available_register();
        compile_maths_expr(mips, equality_expr->what_right(), symbol_table, regs, context, scope, reg);
        std::cerr << equality_expr->what_op() << std::endl;
        if (equality_expr->what_op() == "==") {
            mips << "\t" << "xor" << "\t\t" << dst_reg << "," << dst_reg << "," << reg << std::endl;
            mips << "\t" << "sltiu" << "\t" << dst_reg << "," << dst_reg << "," << "1" << std::endl;
        }
        if (equality_expr->what_op() == "!=") {
            mips << "\t" << "xor" << "\t\t" << dst_reg << "," << dst_reg << "," << reg << std::endl;
            mips << "\t" << "sltu" << "\t" << dst_reg << "," << "$0" << "," << dst_reg << std::endl;
        }
        regs.empty_used_register(reg);
    }
    else if (maths->what_type() == "And_Expression") {
        const And_Expression *and_expr = dynamic_cast<const And_Expression *>(maths);
        compile_maths_expr(mips, and_expr->what_left(), symbol_table, regs, context, scope, dst_reg);
        std::string reg = regs.use_available_register();
        compile_maths_expr(mips, and_expr->what_right(), symbol_table, regs, context, scope, reg);
        mips << "\t" << "and" << "\t" << dst_reg << "," << dst_reg << "," << reg << std::endl;
        regs.empty_used_register(reg);
    }
    else if (maths->what_type() == "Exclusive_Or_Expression") {
        const Exclusive_Or_Expression *exclusive_or_expr = dynamic_cast<const Exclusive_Or_Expression *>(maths);
        compile_maths_expr(mips, exclusive_or_expr->what_left(), symbol_table, regs, context, scope, dst_reg);
        std::string reg = regs.use_available_register();
        compile_maths_expr(mips, exclusive_or_expr->what_right(), symbol_table, regs, context, scope, reg);
        mips << "\t" << "xor" << "\t\t" << dst_reg << "," << dst_reg << "," << reg << std::endl;
        regs.empty_used_register(reg);
    }
    else if (maths->what_type() == "Inclusive_Or_Expression") {
        const Inclusive_Or_Expression *inclusive_or_expr = dynamic_cast<const Inclusive_Or_Expression *>(maths);
        compile_maths_expr(mips, inclusive_or_expr->what_left(), symbol_table, regs, context, scope, dst_reg);
        std::string reg = regs.use_available_register();
        compile_maths_expr(mips, inclusive_or_expr->what_right(), symbol_table, regs, context, scope, reg);
        mips << "\t" << "or" << "\t" << dst_reg << "," << dst_reg << "," << reg << std::endl;
        regs.empty_used_register(reg);
    }
    else if (maths->what_type() == "Logical_And_Expression") {
        const Logical_And_Expression* logical_and_expression = dynamic_cast<const Logical_And_Expression*>(maths);
        std::string end_and_id = Helper::generate_unique_id("end_and");
        compile_maths_expr(mips, logical_and_expression->what_left(), symbol_table, regs, context, scope, dst_reg);
        mips << "\t" << "beq" << "\t" << dst_reg << ",$0," << end_and_id << "\t# If short_circuit. "<< std::endl; 
        std::string reg = regs.use_available_register();
        compile_maths_expr(mips, logical_and_expression->what_right(), symbol_table, regs, context, scope, reg);
        std::string return_zero_id = Helper::generate_unique_id("return_zero");
        mips << "\t" << "beq" << "\t\t" << dst_reg << ",$0," << return_zero_id << std::endl;
        mips << "\t" << "beq" << "\t\t" << reg << ",$0," << return_zero_id << std::endl;
        mips << "\t" << "nop" << std::endl;
        mips << "\t" << "li" << "\t\t" << dst_reg << ",1" << std::endl;
        mips << "\t" << "b" << "\t\t" << end_and_id << std::endl;
        mips << "\t" << "nop" << std::endl;
        mips << return_zero_id << ":" << std::endl;
        mips << "\t" << "move" << "\t" << dst_reg << ",$0" << std::endl;
        mips << end_and_id << ":" << std::endl;
        regs.empty_used_register(reg);
    }
    else if (maths->what_type() == "Logical_Or_Expression") {
        const Logical_Or_Expression* logical_or_expression = dynamic_cast<const Logical_Or_Expression*>(maths);
        std::string end_or_id = Helper::generate_unique_id("end_or");
        compile_maths_expr(mips, logical_or_expression->what_left(), symbol_table, regs, context, scope, dst_reg);
        std::string reg = regs.use_available_register();
        compile_maths_expr(mips, logical_or_expression->what_right(), symbol_table, regs, context, scope, reg);
        std::string return_one_id = Helper::generate_unique_id("return_one");
        mips << "\t" << "bne" << "\t\t" << dst_reg << ",$0," << return_one_id << std::endl;
        mips << "\t" << "nop" << std::endl;
        mips << "\t" << "bne" << "\t\t" << reg << ",$0," <<return_one_id << std::endl;
        mips << "\t" << "nop" << std::endl;
        mips << "\t" << "move" << "\t" << dst_reg << ",$0" << std::endl;
        mips << "\t" << "b" << "\t\t" << end_or_id << std::endl;
        mips << "\t" << "nop" << std::endl;
        mips << return_one_id << ":" << std::endl;
        mips << "\t" << "li" << "\t\t" << dst_reg << ",1" << std::endl;
        mips << end_or_id << ":" << std::endl;
        regs.empty_used_register(reg);
    }
    else if (maths->what_type() == "Conditional_Expression") {
        const Conditional_Expression* cond_expr = dynamic_cast<const Conditional_Expression *>(maths);
        std::string reg_true = regs.use_available_register();
        compile_maths_expr(mips, cond_expr->what_cond(), symbol_table, regs, context, scope, dst_reg);
        std::string end_cond_id = Helper::generate_unique_id("end_cond");
        mips << "\t" << "beq" << "\t\t" << dst_reg << ",$0," <<end_cond_id << std::endl;
        mips << "\t" << "nop" << std::endl;
        compile_maths_expr(mips,cond_expr->what_expr_true(), symbol_table, regs, context, scope, reg_true);
        compile_maths_expr(mips,cond_expr->what_expr_false(), symbol_table, regs, context, scope, dst_reg);
        mips << "\t" << "move" << "\t" << dst_reg << "," << reg_true << std::endl;
        mips << end_cond_id << ":" << std::endl;
        regs.empty_used_register(reg_true);
    }
    else if (maths->what_type() == "Assignment_Expression") {
        const Assignment_Expression *assign_expr = dynamic_cast<const Assignment_Expression *>(maths);
        compile_assign_expr(mips, assign_expr, symbol_table, regs, context, scope, dst_reg);
    }
    else if (maths->what_type() == "Function_Call") {
        std::cerr << "[compile_maths_expr] : Function_Call" << std::endl;
        const Function_Call *func_call = dynamic_cast<const Function_Call*>(maths);
        compile_func_call(mips, func_call, symbol_table, regs, context, scope, dst_reg);
    }
    else if (maths->what_type() == "Sizeof_Expression") {
        std::cerr << "SIZEOF" << std::endl;
        const Sizeof_Expression *sizeof_expr = dynamic_cast<const Sizeof_Expression *>(maths);
        const Variable *var = dynamic_cast<const Variable *>(sizeof_expr->what_var());
        std::cerr << symbol_table->what_type(var->what_var()) << std::endl;
        if (symbol_table->what_type(var->what_var()) == "int") {
            mips << "\t" << "li" << "\t\t" << dst_reg << ",4" << std::endl;
        }
        else if (symbol_table->what_type(var->what_var()) == "char") {
            mips << "\t" << "li" << "\t\t" << dst_reg << ",1" << std::endl;
        }
    }
}

void load_reg(std::ostream &mips, const Variable *var, Symbol_Table *symbol_table, Register &regs, Context &context, std::string scope, const std::string &dst_reg) {
    std::cerr << "[LOAD REGISTER]" << std::endl;
    if (context.is_var_local(var->what_var())) {
        if (var->what_type() == "Variable_Normal") {
            int offset = context.what_var_offset(var->what_var());
            std::cerr << "[LOAD REGISTER]: offset " << offset << std::endl;
            mips << "\t" << "lw" << "\t\t" << dst_reg << "," << offset << "($fp)" << std::endl;
        }
        else if (var->what_type() == "Variable_Array") {
            int base_address = context.get_array_base_offset(var->what_var());
            const std::string& offset_reg = regs.use_available_register();
            compile_maths_expr(mips, var->what_index_array(), symbol_table, regs, context, scope, offset_reg);
            mips << "sll\t " << offset_reg << "," << offset_reg << ", 2" << std::endl;
            mips << "addiu\t " << dst_reg << "," << offset_reg << "," << base_address  << std::endl;
            mips << "addu\t " << dst_reg << "," << dst_reg << ",$fp"  << std::endl;
            mips << "lw\t " << dst_reg << ",0(" << dst_reg << ")" << std::endl;
            mips << "nop" << std::endl;
            regs.empty_used_register(offset_reg);
        }
        else if (var->what_type() == "Variable_Pointer") {
            int offset = context.what_var_offset(var->what_var());
            const std::string ptrreg = regs.use_available_register();
            mips << "lw\t " << ptrreg << "," << offset << "($fp)" <<  std::endl;
            mips << "lw\t " << dst_reg << ",0(" << ptrreg <<")" << std::endl;
            regs.empty_used_register(ptrreg);
        }
    }
    else if (global_var.is_var_global(var->what_var())) {
        if (var->what_type() == "Variable_Normal") {
            int offset = context.what_var_offset(var->what_var());
            mips << "\t" << "lui" << "\t\t" << dst_reg << ",%hi(" << var->what_var() << ")" << std::endl;
            mips << "\t" << "lw" << "\t\t" << dst_reg << ",%lo(" << var->what_var() << ")" << "(" << dst_reg << ")" << std::endl;
            mips << "\t" << "nop" << std::endl;
        }
        else if (var->what_type() == "Variable_Array") {
            mips << "lui\t " << dst_reg << ",%hi(" << var->what_var() << ")" << std::endl;
            mips << "addiu\t " << dst_reg << "," << dst_reg << ",%lo(" << var->what_var() << ")" << std::endl;
            const std::string& offset_reg = regs.use_available_register();
            compile_maths_expr(mips, var->what_index_array(), symbol_table, regs, context, scope, offset_reg);
            mips << "sll\t " << offset_reg << "," << offset_reg << ",2" << std::endl;
            mips << "addu\t " << dst_reg << "," << dst_reg << "," << offset_reg << std::endl;
            mips << "lw\t " << dst_reg << ",0(" << dst_reg << ")" <<  std::endl;
            mips << "nop"  << std::endl;
            regs.empty_used_register(offset_reg);
        }
        else if (var->what_type() == "Variable_Pointer") {
            mips << "lui\t " << dst_reg << ",%hi(" << var->what_var() << ")" <<  std::endl;
            mips << "lw\t " << dst_reg << ",%lo(" << var->what_var() << ")" << "(" << dst_reg << ")" <<  std::endl;
            mips << "nop" <<  std::endl;
            mips << "lw\t " << dst_reg << ",0(" << dst_reg << ")" << std::endl;
            mips << "nop" << std::endl;
        }
    }
}

void store_reg(std::ostream &mips, const Variable *var, Symbol_Table *symbol_table, Register &regs, Context &context, std::string scope, const std::string &src_reg, bool is_decl) {
    std::cerr << "[STORE REGISTER]" << std::endl;
    if (context.is_var_local(var->what_var()) || is_decl ) {
        if (var->what_type() == "Variable_Normal") {
            int offset = context.store_in_stack(var->what_var(), scope, is_decl);
            std::cerr << "[STORE REGISTER]: offset " << offset << std::endl;
            mips << "\t" << "sw" << "\t\t" << src_reg << "," << offset << "($fp)" << std::endl;
        }
        else if (var->what_type() == "Variable_Array") {
            int base_address = context.get_array_base_offset(var->what_type());
            const std::string& offset_reg = regs.use_available_register();
            compile_maths_expr(mips, var->what_index_array(), symbol_table, regs, context, scope, offset_reg);
            mips << "sll\t " << offset_reg << "," << offset_reg << ",2" << std::endl;
            mips << "addiu\t " << offset_reg << "," << offset_reg << "," << base_address << std::endl;
            mips << "sw\t " << src_reg << ",0(" << offset_reg << ")" << std::endl;
            mips << "nop" << std::endl;
            regs.empty_used_register(offset_reg);
        }
        else if (var->what_type() == "Variable_Pointer") {
            int offset = context.what_var_offset(var->what_type());
            const std::string ptrreg = regs.use_available_register();
            mips << "lw\t " << ptrreg << "," << offset << "($fp)"  << std::endl;
            mips << "sw\t " << src_reg << ",0(" << ptrreg <<")"  << std::endl;
            regs.empty_used_register(ptrreg);
        }
    }
    else if (global_var.is_var_global(var->what_var())) {
        if (var->what_type() == "Variable_Normal") {
            const std::string& addr_reg = regs.use_available_register();
            mips << "lui\t " << addr_reg << ",%hi(" << var->what_type() << ")" << std::endl;
            mips << "sw\t " << src_reg << ",%lo(" << var->what_type() << ")" << "(" << addr_reg << ")" << std::endl;
            mips << "nop" <<  std::endl;
            regs.empty_used_register(addr_reg);
        }
        else if (var->what_type() == "Variable_Array") {
            const std::string& addr_reg = regs.use_available_register();

            mips << "lui\t " << addr_reg << ",%hi(" << var->what_type() << ")" << std::endl;
            mips << "addiu\t " << addr_reg << "," << addr_reg << ",%lo(" << var->what_type() << std::endl;
            const std::string& offset_reg = regs.use_available_register();
            compile_maths_expr(mips, var->what_index_array(), symbol_table, regs, context, scope, offset_reg);
            mips << "sll\t " << offset_reg << "," << offset_reg << ",2" << std::endl;
            mips << "addu\t " << addr_reg << "," << addr_reg << "," << offset_reg <<  std::endl;
            mips << "sw\t " << src_reg << ",0(" << addr_reg << ")" << std::endl;
            mips << "nop" <<  std::endl;
            regs.empty_used_register(addr_reg);
            regs.empty_used_register(offset_reg);
        }
        else if (var->what_type() == "Variable_Pointer") {
            const std::string ptr_reg = regs.use_available_register();
            mips << "lui\t " << ptr_reg << ",%hi(" << var->what_type() << ")" << std::endl;
            mips << "lw\t " << ptr_reg << ",%lo(" << var->what_type() << ")" << "(" << ptr_reg << ")" << std::endl;
            mips << "nop" << std::endl;
            mips << "sw\t " << src_reg << ",0(" << ptr_reg << ")" << std::endl;
            mips << "nop" <<  std::endl;
            regs.empty_used_register(ptr_reg);
        }
    }
}


// --------------------------------------

void generate_global_variable(Symbol_Table *symbol_table) {
    //std::cout << ".data\n";
    //std::cout << "# Global Variables\n";

    std::vector<std::string> global_variables = symbol_table->find_global();
    for(std::string global_var : global_variables) {
        if(symbol_table->what_value(global_var) != "unknown") {
            std::cout << global_var << ":\t" << ".word\t" << symbol_table->what_value(global_var) << std::endl;
        }
    }
}

void generate_function_frame(Symbol_Table *symbol_table) {
    std::vector<std::string> functions = symbol_table->find_function();
    for(std::string function : functions) {

        int depth = std::stoi(symbol_table->what_value(function)), stack_frame;
        if (depth == 0) {
            stack_frame = 8;
        }
        else {
            stack_frame = 16;
            while(depth>0) {
                stack_frame += 8;
                depth-=2; 
            }
        }

        std::cout << function << ":" << std::endl;
        std::cout << "\t" << "addiu\t" << "$sp,$sp," << -stack_frame << std::endl;
        std::cout << "\t" << "sw\t" << "$fp," << stack_frame-4 << "($sp)" << std::endl;
        std::cout << "\t" << "move\t" << "$fp,$sp" << std::endl;

        // Function Assembly

        std::cout << "\t" << "move\t" << "$sp,fp" << std::endl;
        std::cout << "\t" << "lw\t" << "$fp," << stack_frame-4 << "($sp)" << std::endl;
        std::cout << "\t" << "addiu\t" << "$sp,$sp," << stack_frame << std::endl;
        std::cout << "\t" << "j\t" << "$31" << std::endl;
        std::cout << "\t" << "nop" << std::endl;
    }
}

#endif