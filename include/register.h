#ifndef COMPILER_V_0_1_REGISTER_H
#define COMPILER_V_0_1_REGISTER_H

#include <memory>

#include "value.h"

struct Register {
    char label;
    bool free;
    std::shared_ptr<Value> value;
};


#endif //COMPILER_V_0_1_REGISTER_H
