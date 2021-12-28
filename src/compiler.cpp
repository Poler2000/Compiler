#include "compiler.h"

#include <iostream>

Compiler::Compiler() {

}

void Compiler::assert_variable_declared(const std::string &val, const unsigned int line) {
    if (!get_var_manager().is_declared(val)) {
        std::cerr << "variable not declared: " << val << " in line: " << line << '\n';
        exit(EXIT_FAILURE);
    }
}

void Compiler::assert_variable_not_declared(const std::string &val, const unsigned int line) {
    if (get_var_manager().is_declared(val)) {
        std::cerr << "redeclaration of variable: " << val << " in line: " << line << '\n';
        exit(EXIT_FAILURE);
    }
}

CodeGenerator &Compiler::get_code_generator() {
    return generator;
}

VariableManager &Compiler::get_var_manager() {
    return varManager;
}

void Compiler::assert_type(const std::string &val, Value::ValueType type, unsigned int line) {
    if (get_var_manager().get(val)->get_type() != type) {
        std::cerr << "Error: incorrect type of var: " << val << " in line: " << line << '\n';
        exit(EXIT_FAILURE);
    }
}

void Compiler::assert_initialized(const std::string &val, unsigned int line) {
    if (!get_var_manager().get(val)->is_initialized()) {
        std::cerr << "Error: variable: " << val << " not initialized in line: " << line << '\n';
        exit(EXIT_FAILURE);
    }
}
