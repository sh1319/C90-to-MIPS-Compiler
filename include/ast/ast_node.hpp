#ifndef ast_node_hpp
#define ast_node_hpp

#include <iostream>
#include <string>
#include <memory>
#include <map>

class Node {
    protected:
        std::string _type;
    public:
        virtual ~Node() {}
        virtual const std::string& what_type() const { return _type; }; 
        virtual std::ostream& print(std::ostream& dst, std::string space) const=0;
        virtual void print_debug(std::ostream& dst) const { dst << _type << std::endl; }
};

#endif