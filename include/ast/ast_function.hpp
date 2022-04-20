#ifndef ast_function_hpp
#define ast_function_hpp

#include "ast_node.hpp"

/*
<Nodes>: 
- Function_Definition, Function_Declaration, Argument_List

                <Function_Definition>
                /       \           \
_type_specifier         \          <Compound_Statement>
               <Function_Declaration>             \
              /      /      |        \          **refer to ast_statement.hpp
        _function  ( <Argument_List> )
                      /           \
                    _arg         <Argument_List>
                                  /           \
                                _arg          _arg_next ...

- Function_Call, Parameter_List
                <Function_Call>
                /          \
            _declarator   <Parameter_List>
                        /              \
                    _param         <Parameter_List>
                                    /           \
                                _param         _param_next ...

*/

// Function Definition and Declaration 
class Function_Declaration : public Node {
    private:
        const Node *_function;
        const Node *_params;
    
    public:
        Function_Declaration(const Node *function, const Node *params) : _function(function), _params(params) { _type = "Function_Declaration"; }
        const Node *what_function() const { return _function; }
        const Node *what_params() const {return _params; }
        virtual std::ostream& print(std::ostream &os, std::string space) const override {
            os << space << "<" << _type << ">" << std::endl;
            if (what_function() != nullptr) { os << space; _function->print(os,space+" "); }
            if (what_params() != nullptr) { os << space; _params->print(os,space+" "); }
            return os;
        }
};
class Function_Definition : public Node {
    private:
        const Node *_type_specifier;
        const Node *_function;
        const Node *_params;
        const Node *_definition;
    public:
        Function_Definition(const Node *type_specifier, const Node* function, const Node* params, const Node* definition) : _type_specifier(type_specifier), _function(function), _params(params), _definition(definition) { _type = "Function_Definition"; }
        const Node *what_type_specifier() const { return _type_specifier; }
        const Node *what_function() const { return _function; }
        const Node *what_params() const { return _params; }
        const Node *what_definition() const { return _definition; }
        virtual std::ostream& print(std::ostream &os, std::string space) const override {
            os << space << "<" << _type << ">" << std::endl;
            if (what_type_specifier() != nullptr) { os << space; _type_specifier->print(os,space+" "); }
            if (what_function() != nullptr) { os << space; _function->print(os,space+" "); }
            if (what_params() != nullptr) { os << space; _params->print(os,space+" "); }
            if (what_definition() != nullptr) { os << space; _definition->print(os,space+" "); }
            return os;
        }
};
class Parameter_List : public Node {
    private:
        const Node *_param_list;
        const Node *_param;
    public:
        Parameter_List(const Node *param_list, const Node *param) : _param_list(param_list), _param(param) { _type = "Parameter_List"; }
        const Node *what_param_list() const { return _param_list; }
        const Node *what_param() const {return _param; }
        virtual std::ostream& print(std::ostream &os, std::string space) const override {
            os << space << "<" << _type << ">" << std::endl;
            if (what_param_list() != nullptr) { os << space; _param_list->print(os,space+" "); }
            if (what_param() != nullptr) { os << space; _param->print(os,space+" "); }
            return os;
        }
};

class Parameter_Declaration : public Node {
    private:
        const Node *_decl_specifiers;
        const Node *_declarator;
    public:
        Parameter_Declaration(const Node *decl_specifiers, const Node *declarator) : _decl_specifiers(decl_specifiers), _declarator(declarator) { _type = "Parameter_Declaration"; }
        const Node *what_decl_specifiers() const { return _decl_specifiers; }
        const Node *what_declarator() const {return _declarator; }
        virtual std::ostream& print(std::ostream &os, std::string space) const override {
            os << space << "<" << _type << ">" << std::endl;
            if (what_decl_specifiers() != nullptr) { os << space; _decl_specifiers->print(os,space+" "); }
            if (what_declarator() != nullptr) { os << space; _declarator->print(os,space+" "); }
            return os;
        }
};


// Arguments are the data that is passed into method's parameters
class Function_Call : public Node {
    private:
        const Node *_declarator;
        const Node *_args;
    public:
        Function_Call(const Node *declarator, const Node *args) : _declarator(declarator), _args(args) { _type = "Function_Call"; }
        const Node *what_declarator() const { return _declarator; }
        const Node *what_args() const { return _args; }
        virtual std::ostream& print(std::ostream &os, std::string space) const override {
            os << space << "<" << _type << ">" << std::endl;
            if (what_declarator() != nullptr) { os << space; _declarator->print(os,space+" "); }
            if (what_args() != nullptr) { os << space; _args->print(os,space+" "); }
            return os;
        }
};
class Argument_List : public Node {
    private:
        const Node *_arg_list;
        const Node *_arg;
    public:
        Argument_List(const Node *argument_list, const Node *argument) : _arg_list(argument_list), _arg(argument) { _type = "Argument_List"; }
        const Node *what_arg_list() const { return _arg_list; }
        const Node *what_arg() const { return _arg; }
        virtual std::ostream& print(std::ostream &os, std::string space) const override {
            os << space << "<" << _type << ">" << std::endl;
            if (what_arg_list() != nullptr) { os << space; _arg_list->print(os,space+" "); }
            if (what_arg() != nullptr) { os << space; _arg->print(os,space+" "); }
            return os;
        }
};


#endif