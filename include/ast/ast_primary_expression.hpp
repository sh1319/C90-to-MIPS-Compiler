#ifndef ast_primary_expression_hpp
#define ast_primary_expression_hpp

#include "ast_node.hpp"

class Variable : public Node {
    private:
        std::string _var;
        std::string _var_type;
        const Node *_index;
    public:
        Variable(const std::string& var, const std::string& var_type, const Node *index) : _var(var), _var_type(var_type), _index(index) { _type = "Variable_" + var_type; }
        const std::string what_var() const { return _var; }
        const Node *what_index_array() const { return _index; }
        virtual std::ostream& print(std::ostream &os, std::string space) const override { 
            os << space << "<" << _type << ">" << std::endl; 
            os << "\t"+space << _var << std::endl; return os; 
        }
};
class Int_Constant : public Node {
    private:
        int _val;
    public:
        Int_Constant(int val) : _val(val) { _type = "Int_Constant"; }
        int what_val() const { return _val; }
        virtual std::ostream& print(std::ostream &os, std::string space) const override { 
            os << space << "<" << _type << ">" << std::endl; 
            os << "\t"+space << _val << std::endl; return os; 
        }
};
class Float_Constant : public Node {
    private:
        float _val;
    public:
        Float_Constant(float val) : _val(val) { _type = "Float_Constant"; }
        float what_val() const { return _val; }
        virtual std::ostream& print(std::ostream &os, std::string space) const override { 
            os << space << "<" << _type << ">" << std::endl; 
            os << "\t"+space << _val << std::endl; return os; 
        }
};
class Char_Constant : public Node {
    private:
        std::string _char;
    public:
        Char_Constant(const std::string& character) : _char(character) { _type = "Char_Constant"; }
        std::string what_val() const { return _char; }
        virtual std::ostream& print(std::ostream &os, std::string space) const override { 
            os << space << "<" << _type << ">" << std::endl; 
            os << "\t"+space << _char << std::endl; return os; 
        }
};
class Str_Constant : public Node {
    private:
        std::string _str;
        int _size;
    public:
        Str_Constant(const std::string &str, int size) : _str(str), _size(size) { _type = "Str_Constant"; }
        std::string what_str() const { return _str; }
        virtual std::ostream &print(std::ostream &os, std::string space) const override {
            os << space << "<" << _type << ">" << std::endl;
            os << "\t"+space << _str << std::endl; return os; 
        }

};

#endif
