#include <iostream>
#include "variable_manager.h"
#include "memory_manager.h"

void VariableManager::declare(LValue *val) {
    val->set_address(MemoryManager::allocate(val->get_size()));
    variableMap[val->getId()] = std::shared_ptr<LValue>(val);
}

void VariableManager::undeclare(LValue *val) {
    variableMap.erase(val->getId());
}

bool VariableManager::is_declared(const std::string &id) {
    return variableMap[id] != nullptr;
}

std::shared_ptr<LValue> VariableManager::get(const std::string &id) {
    if (!is_declared(id)) {
        std::cerr << "ERROR: attempted to use undeclared variable: " << id << '\n';
        exit(EXIT_FAILURE);
    }

    return variableMap[id];
}
