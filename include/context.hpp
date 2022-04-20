#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include "ast.hpp"

#include <unordered_map>
#include <unordered_set>
#include <stack>

class Global_Var {
    private:
        std::unordered_map<std::string, std::string> _map_id_info;
    
    public:
        void add_global_var(const std::string& id, const std::string& info) { _map_id_info.insert(std::pair<std::string, std::string>(id, info)); }
        bool is_var_global(const std::string& id) const{ return _map_id_info.find(id) != _map_id_info.end(); }
        const std::string& var_info(const std::string& id) const { return _map_id_info.at(id); }
        const std::vector<std::string> all_global_var_id() const{ 
            std::vector<std::string> ids;
            for (std::pair<std::string, std::string> id_to_info : _map_id_info) {
                ids.push_back(id_to_info.first);
            }
            return ids;
        }
};

class Function_Decl {
    private:
        std::unordered_set<std::string> _id_list;
    
    public:
        bool is_id_no_def(const std::string& id){ return _id_list.find(id) != _id_list.end(); }
        void add_id(const std::string& id) { _id_list.insert(id); }
};

class Hash {
    public:
        template <class T1, class T2>
        std::size_t operator () (const std::pair<T1,T2> &p) const {
            auto h1 = std::hash<T1>{}(p.first);
            auto h2 = std::hash<T2>{}(p.second);
            return h1 ^ h2;
        }
};

class Context {
    protected:
        int _size_of_frame;
        std::unordered_map<std::pair<std::string, std::string>, int, Hash> _var_to_offset;
        std::unordered_map<int, std::pair<std::string, std::string>> _offset_to_var;
        std::vector<std::string> _scope_list;
        std::stack<std::string> _break_label;
        std::stack<std::string> _continue_label;
        std::stack<std::string> _default_label;
        std::string _function_epilogue_label;
        const int _word_length = 4;
        int _arg_size_call = 4 * _word_length;

    public:
        Context(int size_of_frame, const std::string& function_epilogue_label) : _size_of_frame(size_of_frame), _function_epilogue_label(function_epilogue_label) {}
        int what_arg_size_call(bool empty_arg) { if (empty_arg) { return 0; } else { return 4*_word_length; } }
        const std::string &what_func_epilogue_label() const { return _function_epilogue_label; }
        const std::string &what_break_label() const { return _break_label.top(); }
        const std::string &what_continue_label() const { return _continue_label.top(); }
        const std::string &what_default_label() const { return _default_label.top(); }
        
        void add_while(const std::string& continue_label, const std::string& break_label){
            _continue_label.push(continue_label);              
            _break_label.push(break_label);              
        }
        void remove_while(){
            _continue_label.pop();
            _break_label.pop();
        }
        void add_for(const std::string& continue_label, const std::string& break_label){
            _continue_label.push(continue_label);              
            _break_label.push(break_label);              
        }
        void remove_for(){
            _continue_label.pop();
            _break_label.pop();
        }
        void add_switch(const std::string& default_label, const std::string& break_label){
            _default_label.push(default_label);
            _break_label.push(break_label);
        }
        void remove_switch(){
            _default_label.pop();
            _break_label.pop();
        }
        
        void add_scope(const std::string& scope_id) { _scope_list.push_back(scope_id); }
        void remove_scope() { _scope_list.pop_back(); }

        int store_in_stack(const std::string& var_name, const std::string& scope_id, const bool& is_decl){
            if (is_decl) {
                std::pair<std::string, std::string> var_scope(var_name, scope_id);
                if (_var_to_offset.find(var_scope) != _var_to_offset.end()) {
                    return _var_to_offset[var_scope];
                }
                for (int i = 8; i < _size_of_frame; i += _word_length) {
                    if (_offset_to_var.find(i) == _offset_to_var.end()) {
                        _var_to_offset.insert(std::pair<std::pair<std::string, std::string>, int>(var_scope, i));
                        _offset_to_var.insert(std::pair<int, std::pair<std::string, std::string>>(i, var_scope));
                        return i;
                    }
                }
            }
            else {
                for (int i = _scope_list.size()-1; i >= 0; i--){
                    std::pair<std::string, std::string> var_scope(var_name, _scope_list[i]);
                    if (_var_to_offset.find(var_scope) != _var_to_offset.end()) {
                        return _var_to_offset[var_scope];
                    }
                }
            }
        }
        int what_var_offset(const std::string& var_name){
            std::cerr << "SCOPE LIST SIZE: " << _scope_list.size() << std::endl;
            for (std::string sc : _scope_list) {
                std::cerr << sc << std::endl;
            }
            for(int i=_scope_list.size()-1; i>=0; i--){
                std::pair<std::string, std::string> var_scope = {var_name, _scope_list[i]};
                if (_var_to_offset.find(var_scope) != _var_to_offset.end()) {
                    return _var_to_offset[var_scope];
                }
            }
        }

        void save_param_offset(const std::string& arg_name, int offset, const std::string& scope_id){
            std::pair<std::string, std::string> arg_scope = {arg_name, scope_id};
            std::cerr << "HOW MANY TIMES??" << std::endl;
            _var_to_offset.insert(std::pair<std::pair<std::string, std::string>, int>(arg_scope, offset));
            _offset_to_var.insert(std::pair<int, std::pair<std::string, std::string>>(offset, arg_scope));
        }

        void reserve_array_space(const std::string& array_name, int size, const std::string& scope_id){
            int start_index = -1;
            for (int i = 0; i < _size_of_frame; i += _word_length) {
                if (_offset_to_var.find(i) == _offset_to_var.end()) {
                    start_index = i;
                    break;
                }
            }
            int position = 0;
            for (int i = start_index; position < size; i += _word_length) {
                std::pair<std::string, std::string> array_scope = {array_name + "@" + std::to_string(position), scope_id};
                _var_to_offset.insert(std::pair<std::pair<std::string, std::string>, int>(array_scope, i));
                _offset_to_var.insert(std::pair<int, std::pair<std::string, std::string>>(i, array_scope));
                position++;
            }
            _var_to_offset.insert(std::pair<std::pair<std::string, std::string>, int> {{array_name, scope_id}, start_index});
        }


        int get_array_base_offset(const std::string& array_name) {
            std::cout << "HERE IS THE SCOPE SIZE: " << _scope_list.size() << std::endl;
            for(int i = _scope_list.size()-1; i >= 0; i--){
                std::string array_base_name = array_name + "@0";
                std::pair<std::string, std::string> array_scope = {array_base_name, _scope_list[i]};
                if (_var_to_offset.find(array_scope) !=_var_to_offset.end()) {
                    return _var_to_offset[array_scope];
                }
            }
            return 0;
        }

        bool is_var_local(const std::string& id){
            for (std::string scope : _scope_list) {
                std::pair<std::string, std::string> key(id, scope);
                if (_var_to_offset.find(key) != _var_to_offset.end()) {
                    return true;
                }
            }   
            return false;
        }
};



#endif