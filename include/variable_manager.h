#ifndef COMPILER_V_0_1_VARIABLE_MANAGER_H
#define COMPILER_V_0_1_VARIABLE_MANAGER_H

#include <memory>
#include <string>
#include <map>

#include "lvalue.h"

class VariableManager {
public:
    void declare(LValue* val);
    void undeclare(LValue* val);
    bool is_declared(const std::string& id);
    std::shared_ptr<LValue> get(const std::string& id);
private:
    std::map<std::string, std::shared_ptr<LValue>> variableMap;
};


#endif //COMPILER_V_0_1_VARIABLE_MANAGER_H
