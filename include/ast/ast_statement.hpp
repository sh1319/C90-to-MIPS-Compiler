#ifndef ast_statements_hpp
#define ast_statements_hpp

#include "ast_node.hpp"

/*
<Nodes>: 
- Compound_Statement, Statement_List

        <Compound_Statement>
        /         |         \
       {  <Statement_List>   }
            /             \
     <Statement_List>   _stmt
        /           \       \
  <Statement_List>  _stmt  ***any Statement can be _stmt (refer to parser)
        ...

- While_Statement
        <While_Statement>
       / |       |      | \
   while ( <Expression> ) _stmt
                 |
            **refer to ast_expression.hpp

- For_Statement
                            <For_Statement>
    /   |        |                      |                    |     \  \
   for  ( <Expression_Statement> <Expression_Statement> <Expression ) _stmt
               |                                             |
            **refer to ast_expression.hpp                   **can be nullptr

- If_Statement
                <If_Statement>
            / /     |      \  \      \     \
          if ( <Expression> ) _stmt else _stmt
                                            |
                                            **can be nullptr

- Return_Statement
                <Return_Statement>
                /       |        \
            return <Expression>   ;
                        |
                    **refer to ast_expression.hpp
*/


class Statement_List : public Node {
    private:
        const Node *_stmt_list;
        const Node *_stmt;
    public:
        Statement_List(const Node *stmt_list, const Node *stmt) : _stmt_list(stmt_list), _stmt(stmt) { _type = "Statement_List"; }
        const Node *what_stmt_list() const { return _stmt_list; }
        const Node *what_stmt() const { return _stmt; }
        virtual std::ostream& print(std::ostream &os, std::string space) const override {
            os << space << "<" << _type << ">" << std::endl;
            if (what_stmt_list() != nullptr) { os << space; _stmt_list->print(os,space+" "); }
            if (what_stmt() != nullptr) { os << space; _stmt->print(os,space+" "); }
            return os;
        }
};
class Compound_Statement : public Node { 
    private:
        const Node *_stmt_list;
        const Node *_decl_list;
    public:
        Compound_Statement(const Node *stmt_list, const Node *decl_list) : _stmt_list(stmt_list), _decl_list(decl_list) { _type = "Compound_Statement"; }
        const Node *what_stmt_list() const { return _stmt_list; }
        const Node *what_decl_list() const { return _decl_list; }
        virtual std::ostream& print(std::ostream &os, std::string space) const override {
            os << space << "<" << _type << ">" << std::endl;
            if (what_stmt_list() != nullptr) { os << space; _stmt_list->print(os,space+" "); }
            if (what_decl_list() != nullptr) { os << space; _stmt_list->print(os,space+" "); }
            return os;
        }
};
class While_Statement : public Node { 
    private:
        const Node *_cond;
        const Node *_stmt;
    public:
        While_Statement(const Node *cond, const Node *stmt) : _cond(cond), _stmt(stmt) { _type = "While_Statement"; }
        const Node *what_cond() const { return _cond; }
        const Node *what_stmt() const { return _stmt; }
        virtual std::ostream& print(std::ostream &os, std::string space) const override {
            os << space << "<" << _type << ">" << std::endl;
            if (what_cond() != nullptr) { os << space; _cond->print(os,space+" "); }
            if (what_stmt() != nullptr) { os << space; _stmt->print(os,space+" "); }
            return os;
        }
};
class For_Statement : public Node {  
    private:
        const Node *_init;
        const Node *_cond;
        const Node *_inc;
        const Node *_stmt;
    public:
        For_Statement(const Node *init, const Node *cond, const Node *inc, const Node *stmt) : _init(init), _cond(cond), _inc(inc), _stmt(stmt) { _type = "For_Statement"; }
        const Node *what_init() const { return _init; }
        const Node *what_cond() const { return _cond; }
        const Node *what_inc() const { return _inc; }
        const Node *what_stmt() const { return _stmt; }
        virtual std::ostream& print(std::ostream &os, std::string space) const override {
            os << space << "<" << _type << ">" << std::endl;
            if (what_init() != nullptr) { os << space; _init->print(os,space+" "); }
            if (what_cond() != nullptr) { os << space; _cond->print(os,space+" "); }
            if (what_inc() != nullptr) { os << space; _inc->print(os,space+" "); }
            if (what_stmt() != nullptr) { os << space; _stmt->print(os,space+" "); }
            return os;
        }
};
class If_Statement : public Node { 
    private:
        const Node *_cond;
        const Node *_stmt_if;
        const Node *_stmt_else;
    public:
        If_Statement(const Node *cond, const Node *stmt_if, const Node *stmt_else) : _cond(cond), _stmt_if(stmt_if), _stmt_else(stmt_else) { _type = "If_Statement"; }
        const Node *what_cond() const { return _cond; }
        const Node *what_stmt_if() const { return _stmt_if; }
        const Node *what_stmt_else() const { return _stmt_else; }
        virtual std::ostream& print(std::ostream &os, std::string space) const override {
            os << space << "<" << _type << ">" << std::endl;
            if (what_cond() != nullptr) { os << space; _cond->print(os,space+" "); }
            if (what_stmt_if() != nullptr) { os << space; _stmt_if->print(os,space+" "); }
            if (what_stmt_else() != nullptr) { os << space; _stmt_else->print(os,space+" "); }
            return os;
        }
};
class Return_Statement : public Node { 
    private:
        const Node *_expr;
    public:
        Return_Statement(const Node* expression) : _expr(expression) { _type = "Return_Statement"; }
        const Node* what_expr() const { return _expr; }
        bool is_expr_empty() const { return _expr == nullptr; }

    virtual std::ostream& print(std::ostream &os, std::string space) const override {
        os << space << "<" << _type << ">" << std::endl;
        if (what_expr() != nullptr) { os << space; _expr->print(os,space+" "); }
        return os;
    }
    
};
class Break_Statement : public Node { 
    public:
        Break_Statement() { _type = "Break_Statement"; }
        virtual std::ostream& print(std::ostream &os, std::string space) const override {
            os << space << "<" << _type << ">" << std::endl;
            return os;
        }
};
class Continue_Statement : public Node { 
    public:
        Continue_Statement() { _type = "Continue_Statement"; }
        virtual std::ostream& print(std::ostream &os, std::string space) const override {
            os << space << "<" << _type << ">" << std::endl;
            return os;
        }
};
class Switch_Statement : public Node { 
    private:
        const Node *_expr;
        const Node *_stmt;
    public:
        Switch_Statement(const Node *expr, const Node *stmt) : _expr(expr), _stmt(stmt) { _type = "Switch_Statement"; }
        const Node *what_expr() const { return _expr; }
        const Node *what_stmt() const { return _stmt; }
        virtual std::ostream& print(std::ostream &os, std::string space) const override {
            os << space << "<" << _type << ">" << std::endl;
            if (what_expr() != nullptr) { os << space; _expr->print(os,space+" "); }
            if (what_stmt() != nullptr) { os << space; _stmt->print(os,space+" "); }
            return os;
        }
};
class Case_Statement_List : public Node { 
    private:
        const Node *_stmt_list;
        const Node *_stmt;
    public:
        Case_Statement_List(const Node *stmt_list, const Node *stmt) : _stmt_list(stmt_list), _stmt(stmt) { _type = "Case_Statement_List"; }
        const Node *what_stmt_list() const { return _stmt_list; }
        const Node *what_stmt() const { return _stmt; }
        virtual std::ostream& print(std::ostream &os, std::string space) const override {
            os << space << "<" << _type << ">" << std::endl;
            if (what_stmt_list() != nullptr) { os << space; _stmt_list->print(os,space+" "); }
            if (what_stmt() != nullptr) { os << space; _stmt->print(os,space+" "); }
            return os;
        }
};
class Case_Statement : public Node {
    private:
        const Node *_expr;
        const Node *_stmt_list;
    public:
        Case_Statement(const Node *expr, const Node *stmt_list) : _expr(expr), _stmt_list(stmt_list) { _type = "Case_Statement"; }
        const Node *what_expr() const { return _expr; }
        const Node *what_stmt_list() const { return _stmt_list; }
        virtual std::ostream& print(std::ostream &os, std::string space) const override {
            os << space << "<" << _type << ">" << std::endl;
            if (what_expr() != nullptr) { os << space; _expr->print(os,space+" "); }
            if (what_stmt_list() != nullptr) { os << space; _stmt_list->print(os,space+" "); }
            return os;
        }
};
class Default_Statement : public Node {
    private:
        const Node *_stmt_list;
    public:
        Default_Statement(const Node *stmt_list) : _stmt_list(stmt_list) { _type = "Default_Statement"; }
        const Node *what_stmt_list() const { return _stmt_list; }
        virtual std::ostream& print(std::ostream &os, std::string space) const override {
            os << space << "<" << _type << ">" << std::endl;
            if (what_stmt_list() != nullptr) { os << space; _stmt_list->print(os,space+" "); }
            return os;
        }

};

#endif