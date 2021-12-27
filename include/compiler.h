#ifndef COMPILER_V_0_1_COMPILER_H
#define COMPILER_V_0_1_COMPILER_H

#include "variable_manager.h"
#include "code_generator.h"

class Compiler {
public:
    void assert_variable_declared(LValue* val);
    void assert_variable_not_declared(LValue* val);
private:
    CodeGenerator generator;
    VariableManager varManager;
};


#endif //COMPILER_V_0_1_COMPILER_H
