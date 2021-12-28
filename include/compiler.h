#ifndef COMPILER_V_0_1_COMPILER_H
#define COMPILER_V_0_1_COMPILER_H

#include "variable_manager.h"
#include "code_generator.h"

class Compiler {
public:
    Compiler();
    void assert_variable_declared(const std::string& val, unsigned int line);
    void assert_variable_not_declared(const std::string& val, unsigned int line);
    void assert_type(const std::string& val, Value::ValueType type, unsigned int line);
    void assert_initialized(const std::string& val, unsigned int line);
    CodeGenerator& get_code_generator();
    VariableManager& get_var_manager();
private:
    CodeGenerator generator;
    VariableManager varManager;
};


#endif //COMPILER_V_0_1_COMPILER_H
