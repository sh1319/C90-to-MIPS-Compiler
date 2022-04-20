#ifndef ALLOCATE_REGISTER_HPP
#define ALLOCATE_REGISTER_HPP

#include "ast.hpp"

class Register {
    private:
        std::vector<bool> _used_reg;
        const int _total_reg = 8;
 
    public:
        Register() { _used_reg = std::vector<bool>(_total_reg, false); }

        std::string use_available_register() {
            for (int i = 0; i < _total_reg; i++) {
                if (!_used_reg[i]) {
                    _used_reg[i] = true;
                    return "$t" + std::to_string(i); 
                }
            }
        }
        void empty_used_register(const std::string& reg){
            int reg_id = std::stoi(reg.substr(2,1));
            _used_reg[reg_id] = false;
        }
        std::vector<std::string> get_unavailable_registers(){
            std::vector<std::string> used_reg;
            for (int i = 0; i < _total_reg; i++) {
                if (_used_reg[i]) {
                    used_reg.push_back("$t" + std::to_string(i)); 
                }
            }
            return used_reg;
        }
};

#endif