#ifndef ast_expression_hpp
#define ast_expression_hpp

#include "ast_node.hpp"

/*
<Nodes>:
- Decl_List, Decl, Decl_Specifier, Type_Specifier, Init_Declarator_List

                <Decl_List>
                        |
                <Decl>
            /                   \  
<Decl_Specifier>     <Init_Declarator_List>
            |                       |
    <Type_Specifier>            <Init_Declarator>
            |                        |          |  \
            string               <Declarator>   =  <Initializer>
                                    |                   \
                                <Direct_Declarator>     <Assignment_Expression>
                                    |                   
                                <Variable>
                                    |
                                    **refer to ast_primary_expression.hpp

_type_specifier  _var = _value
                            |
                            **can be nullptr

- Assignment Expression

- Condition_Expression, Expression, Blank_Expression
*/

class Declaration_List : public Node {
    private:
        const Node* _decl_list;
        const Node* _decl;
    public:
        Declaration_List(const Node* decl_list, const Node* decl) : _decl_list(decl_list), _decl(decl) { _type = "Declaration_List"; }
        const Node *what_decl_list() const { return _decl_list; }
        const Node *what_decl() const { return _decl; }
        virtual std::ostream& print(std::ostream &os, std::string space) const override {
            os << space << "<" << _type << ">" << std::endl;
            if(_decl_list != nullptr) { os << space; _decl_list->print(os,space+" "); }
            if(_decl != nullptr) { os << space; _decl->print(os,space+" "); }
            return os;
        }
};

class Declaration : public Node {
    private:
        const Node *_decl_specifier;
        const Node *_init_declarator_list;
    public:
        Declaration(const Node* decl_specifier, const Node* init_declarator_list) : _decl_specifier(decl_specifier),_init_declarator_list(init_declarator_list) { _type = "Declaration"; }
        const Node *what_decl_specifier() const { return _decl_specifier; }
        const Node *what_init_declarator_list() const { return _init_declarator_list; }
        virtual std::ostream& print(std::ostream &os, std::string space) const override {
            os << space << "<" << _type << ">" << std::endl;
            if (what_decl_specifier() != nullptr) { os << space; _decl_specifier->print(os,space+" "); }
            if (what_init_declarator_list() != nullptr) { os << space; _init_declarator_list->print(os,space+" "); }
            return os;
        }
};

class Init_Declarator_List : public Node {
    private:
        const Node* _init_declarator_list;
        const Node* _init_declarator;
    public:
        Init_Declarator_List(const Node* init_declarator_list, const Node* init_declarator) : _init_declarator_list(init_declarator_list), _init_declarator(init_declarator) { _type = "Init_Declarator_List"; };
        const Node *what_init_declarator_list() const { return _init_declarator_list; }
        const Node *what_init_declarator() const { return _init_declarator; }
        virtual std::ostream& print(std::ostream &os, std::string space) const override {
            os << space << "<" << _type << ">" << std::endl;
            if (what_init_declarator_list() != nullptr) { os << space; _init_declarator_list->print(os,space+" "); }
            if (what_init_declarator() != nullptr) { os << space; _init_declarator->print(os,space+" "); }
            return os;
        }
};

class Declaration_Specifiers : public Node {
    private:
        const std::string _decl_specifiers;
    public:
        Declaration_Specifiers(const std::string& decl_specifiers) : _decl_specifiers(decl_specifiers) { _type = "Declaration_Specifiers"; }
        const std::string what_decl_specifiers() const { return _decl_specifiers; }
        virtual std::ostream& print(std::ostream &os, std::string space) const override {
            os << space << "<" << _type << ">" << std::endl;
            os << "\t" << _decl_specifiers << std::endl;
            return os;
        }
};

class Declarator : public Node {
    private:
        const Node *_declarator;
        const Node *_initializer;
    public:
        Declarator(const Node* declarator, const Node* initializer) : _declarator(declarator), _initializer(initializer) { _type = "Declarator"; }       
        const Node *what_declarator() const { return _declarator; }          
        const Node *what_initializer() const { return _initializer; }              
        virtual std::ostream& print(std::ostream &os, std::string space) const override {
            os << space << "<" << _type << ">" << std::endl;
            if (what_declarator() != nullptr) { os << space; _declarator->print(os,space+" "); }
            if (what_initializer() != nullptr) { os << space; _initializer->print(os,space+" "); }
            return os;
        }
};

class Postfix_Expression : public Node {
    private:
        const Node *_postfix_expr;
        std::string _op;
        std::string _name;

    public:
        Postfix_Expression(const Node *postfix_expr, const std::string& op, const std::string& name) : _postfix_expr(postfix_expr), _op(op), _name(name) { _type = "Postfix_Expression"; }
        const Node *what_postfix_expr() const { return _postfix_expr; }
        const std::string what_op() const { return _op; }
        const std::string what_name() const { return _name; }
        virtual std::ostream& print(std::ostream &os, std::string space) const override {
            os << space << "<" << _type << ">" << std::endl;
            if (what_postfix_expr() != nullptr) { os << space; _postfix_expr->print(os,space+" "); }
            os << space << _op << std::endl;
            os << "\t" << _name << std::endl;            
            return os;
        }
};

/*
class Operator_Expression : public Node {
    private:
        const Node *_left;
        const Node *_right;
        std::string operator;
    public:
        virtual ~Operator_Expression();
        virtual const what_operator();
}
*/

class Unary_Expression : public Node {
    private:
        std::string _op;
        const Node* _unary_expr;
    public:
        Unary_Expression(std::string op, const Node* unary_expr) : _op(op), _unary_expr(unary_expr) { _type = "Unary_Expression"; }
        std::string what_op() const { return _op; }
        const Node* what_unary_expr() const { return _unary_expr; }
        virtual std::ostream& print(std::ostream &os, std::string space) const override {
            os << space << "<" << _type << ">" << std::endl;
            os << space << _op << std::endl;
            if (what_unary_expr() != nullptr) { os << space; _unary_expr->print(os,space+" "); }          
            return os;
        }
};

/*
class Operator_Expression : public Node {
    protected:
        const Node *_left;
        std::string _op;
        const Node *_right;
    public:
        Operator_Expression(const Node *left, std::string op, const Node *right) : _left(left), _op(op), _right(right) { 
            _type = "Operator_Expression" + _op;
            if (op == "*")
                _type = "Multiplicative_Expression";
            else if (op == "+")
                _type = "Additive_Expression";
            else if (op == "+")
                _type = "Shift_Expression";
            else if (op == "+")
                _type = "Additive_Expression";
        }
        const Node* what_left() const { return _left; }
        const std::string what_op() const { return _op; }
        const Node* what_right() const { return _right; }
        virtual std::ostream& print(std::ostream &os, std::string space) const override {
            os << "\t" << _type << " [" << std::endl;
            _left->print(os,space+" ");
            os << "\t" << "  type: " << _op << std::endl;
            _right->print(os,space+" ");
            os << "\n\t]";
            return os;
        }
};
*/
class Multiplicative_Expression : public Node {
    private:
        const Node* _left;
        std::string _op;
        const Node* _right;
    public:
        Multiplicative_Expression(const Node* left, const std::string& op, const Node* right) : _left(left), _op(op), _right(right) { _type = "Multiplicative_Expression"; }
        const Node* what_left() const { return _left; }
        std::string what_op() const { return _op; }
        const Node* what_right() const { return _right; }
        virtual std::ostream& print(std::ostream &os, std::string space) const override {
            os << space << "<" << _type << ">" << std::endl;
            if (what_left() != nullptr) { os << space; _left->print(os,space+" ");  }
            os << space << _op << std::endl;
            if (what_right() != nullptr) { os << space; _right->print(os,space+" "); }             
            return os;
        }
};

class Additive_Expression : public Node {
    private:
        const Node* _left;
        std::string _op;
        const Node* _right;
    public:
        Additive_Expression(const Node* left, const std::string& op, const Node* right) : _left(left), _op(op), _right(right) { _type = "Additive_Expression"; }
        const Node* what_left() const { return _left; }
        std::string what_op() const { return _op; }
        const Node* what_right() const { return _right; }
        virtual std::ostream& print(std::ostream &os, std::string space) const override {
            os << space << "<" << _type << ">" << std::endl;
            if (what_left() != nullptr) { os << space; _left->print(os,space+" ");  }
            os << space << _op << std::endl;
            if (what_right() != nullptr) { os << space; _right->print(os,space+" "); }              
            return os;
        }
};

class Shift_Expression : public Node {
    private:
        const Node* _left;
        std::string _op;
        const Node* _right;
    public:
        Shift_Expression(const Node* left, const std::string& op, const Node* right) : _left(left), _op(op), _right(right) { _type = "Shift_Expression"; }
        const Node* what_left() const { return _left; }
        std::string what_op() const { return _op; }
        const Node* what_right() const { return _right; }
        virtual std::ostream& print(std::ostream &os, std::string space) const override {
            os << space << "<" << _type << ">" << std::endl;
            if (what_left() != nullptr) { os << space; _left->print(os,space+" ");  }
            os << space << _op << std::endl;
            if (what_right() != nullptr) { os << space; _right->print(os,space+" "); }           
            return os;
        }
};

class Relational_Expression : public Node {
    private:
        const Node* _left;
        std::string _op;
        const Node* _right;
    public:
        Relational_Expression(const Node* left, const std::string& op, const Node* right) : _left(left), _op(op), _right(right) { _type = "Relational_Expression"; }
        const Node* what_left() const { return _left; }
        std::string what_op() const { return _op; }
        const Node* what_right() const { return _right; }
        virtual std::ostream& print(std::ostream &os, std::string space) const override {
            os << space << "<" << _type << ">" << std::endl;
            if (what_left() != nullptr) { os << space; _left->print(os,space+" ");  }
            os << space << _op << std::endl;
            if (what_right() != nullptr) { os << space; _right->print(os,space+" "); }              
            return os;
        }
};

class Equality_Expression : public Node {
    private:
        const Node* _left;
        std::string _op;
        const Node* _right;
    public:
        Equality_Expression(const Node* left, const std::string& op, const Node* right) : _left(left), _op(op), _right(right) { _type = "Equality_Expression"; }
        const Node* what_left() const { return _left; }
        std::string what_op() const { return _op; }
        const Node* what_right() const { return _right; }
        virtual std::ostream& print(std::ostream &os, std::string space) const override {
            os << space << "<" << _type << ">" << std::endl;
            if (what_left() != nullptr) { os << space; _left->print(os,space+" ");  }
            os << space+"\t" << _op << std::endl;
            if (what_right() != nullptr) { os << space; _right->print(os,space+" "); }    
            return os;
        }
};

class And_Expression : public Node {
    private:
        const Node* _left;
        const Node* _right;
    public:
        And_Expression(const Node* left, const Node* right) : _left(left), _right(right) { _type = "And_Expression"; }
        const Node* what_left() const { return _left; }
        const Node* what_right() const { return _right; }
        virtual std::ostream& print(std::ostream &os, std::string space) const override {
            os << space << "<" << _type << ">" << std::endl;
            if (what_left() != nullptr) { os << space; _left->print(os,space+" ");   }
            os << space << "&" << std::endl;
            if (what_right() != nullptr) { os << space; _right->print(os,space+" ");   }     
            return os;
        }
};

class Exclusive_Or_Expression : public Node {
    private:
        const Node* _left;
        const Node* _right;
    public:
        Exclusive_Or_Expression(const Node* left, const Node* right) : _left(left), _right(right) { _type = "Exclusive_Or_Expression"; }
        const Node* what_left() const { return _left; }
        const Node* what_right() const { return _right; }
        virtual std::ostream& print(std::ostream &os, std::string space) const override {
            os << space << "<" << _type << ">" << std::endl;
            if (what_left() != nullptr) { os << space; _left->print(os,space+" ");   }
            os << space << "^" << std::endl;
            if (what_right() != nullptr) { os << space; _right->print(os,space+" ");   }     
            return os;
        }
};

class Inclusive_Or_Expression : public Node {
    private:
        const Node* _left;
        const Node* _right;
    public:
        Inclusive_Or_Expression(const Node* left, const Node* right) : _left(left), _right(right) { _type = "Inclusive_Or_Expression"; }
        const Node* what_left() const { return _left; }
        const Node* what_right() const { return _right; }
        virtual std::ostream& print(std::ostream &os, std::string space) const override {
            os << space << "<" << _type << ">" << std::endl;
            if (what_left() != nullptr) { os << space; _left->print(os,space+" ");   }
            os << space << "|" << std::endl;
            if (what_right() != nullptr) { os << space; _right->print(os,space+" ");   }     
            return os;
        }
};

class Logical_And_Expression : public Node {
    private:
        const Node* _left;
        const Node* _right;
    public:
        Logical_And_Expression(const Node* left, const Node* right) : _left(left), _right(right) { _type = "Logical_And_Expression"; }
        const Node* what_left() const { return _left; }
        const Node* what_right() const { return _right; }
        virtual std::ostream& print(std::ostream &os, std::string space) const override {
            os << space << "<" << _type << ">" << std::endl;
            if (what_left() != nullptr) { os << space; _left->print(os,space+" ");   }
            os << space << "&&" << std::endl;
            if (what_right() != nullptr) { os << space; _right->print(os,space+" ");   }              
            return os;
        }
};

class Logical_Or_Expression : public Node {
    private:
        const Node* _left;
        const Node* _right;
    public:
        Logical_Or_Expression(const Node* left, const Node* right) : _left(left), _right(right) { _type = "Logical_Or_Expression"; }
        const Node* what_left() const { return _left; }
        const Node* what_right() const { return _right; }
        virtual std::ostream& print(std::ostream &os, std::string space) const override {
            os << space << "<" << _type << ">" << std::endl;
            if (what_left() != nullptr) { os << space; _left->print(os,space+" ");   }
            os << space << "||" << std::endl;
            if (what_right() != nullptr) { os << space; _right->print(os,space+" ");   }        
            return os;
        }
};

class Conditional_Expression : public Node {
    private:
        const Node *_cond;
        const Node *_expr_true;
        const Node *_expr_false;
    public:
        Conditional_Expression(const Node *condition, const Node *expression_true, const Node *expression_false) : _cond(condition), _expr_true(expression_true), _expr_false(expression_false) { _type = "Conditional_Expression"; }
        const Node *what_cond() const { return _cond; }
        const Node *what_expr_true() const { return _expr_true; }
        const Node *what_expr_false() const { return _expr_false; }
        virtual std::ostream& print(std::ostream &os, std::string space) const override {
            os << space << "<" << _type << ">" << std::endl;
            if (what_cond() != nullptr) { os << space; _cond->print(os,space+" "); }
            if(what_expr_true() != nullptr) { os << "\t" << "?" << std::endl; _expr_true->print(os,space+" "); }
            if(what_expr_false() != nullptr) { os << "\t" << ":" << std::endl; _expr_false->print(os,space+" "); }
            return os;
        }
};

class Assignment_Expression : public Node {
    private:
        const Node* _var;
        std::string _op;
        const Node* _value;
    public:
        Assignment_Expression(const Node* variable, const std::string& op, const Node* value) : _var(variable), _op(op), _value(value) { _type = "Assignment_Expression"; }
        const Node* what_var() const { return _var; }
        const std::string& what_op() const { return _op; }
        const Node* what_val() const { return _value; }
        virtual std::ostream& print(std::ostream &os, std::string space) const override {
            os << space << "<" << _type << ">" << std::endl;
            if (what_var() != nullptr) { os << space; _var->print(os,space+" ");  } 
            os << space+"\t" << what_op() << std::endl;
            if (what_val() != nullptr) { os << space; _value->print(os,space+" ");   } 
            return os;
        }
};

class Empty_Expression : public Node {
 public:
  Empty_Expression() { _type = "Empty_Expression";}
  virtual std::ostream& print(std::ostream &os, std::string space) const override {
    os << space << "<" << _type << ">" << std::endl;
    return os;
  }
};

class Enum_Declaration : public Node {
    private:
        std::string _name;
        const Node *_val;
    public:
        Enum_Declaration(const std::string &name, const Node * val) : _name(name), _val(val) { _type = "Enum_Declaration"; }
        const std::string &what_name() { return _name; }
        const Node *what_val() const { return _val; }
        virtual std::ostream& print(std::ostream &os, std::string space) const override {
            os << space << "<" << _type << ">" << std::endl;
            os << space << _name << std::endl;
            if (what_val() != nullptr) { os << space; _val->print(os,space+" ");   } 
            return os;
        }
};

class Enum_Declaration_List : public Node {
    private:
        const Node *_enum_decl_list;
        const Node *_enum_decl;
    public:
        Enum_Declaration_List(const Node *enum_decl_list, const Node *enum_decl) : _enum_decl_list(enum_decl_list), _enum_decl(enum_decl) { _type = "Enum_Declaration_List"; }
        const Node *what_enum_decl_list() const { return _enum_decl_list; }
        const Node *what_enum_decl() const { return _enum_decl; }
        virtual std::ostream& print(std::ostream &os, std::string space) const override {
            os << space << "<" << _type << ">" << std::endl;
            if (what_enum_decl_list() != nullptr) { os << space; _enum_decl_list->print(os,space+" ");   } 
            if (what_enum_decl() != nullptr) { os << space; _enum_decl->print(os,space+" ");   } 
            return os;
        }
};

class Sizeof_Expression : public Node {
    private:
        const Node *_var;
    public:
        Sizeof_Expression(const Node *var) : _var(var) { _type = "Sizeof_Expression"; }
        const Node *what_var() const { return _var; }
        virtual std::ostream& print(std::ostream &os, std::string space) const override {
            os << space << "<" << _type << ">" << std::endl;
            if (what_var() != nullptr) { os << space; what_var()->print(os,space+" ");   } 
            return os;
        }
};

#endif