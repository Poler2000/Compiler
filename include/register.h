#ifndef COMPILER_V_0_1_REGISTER_H
#define COMPILER_V_0_1_REGISTER_H

#include <memory>

#include "value.h"

struct Register {
    char label;
    bool free;
    Value const* value;
};


#endif //COMPILER_V_0_1_REGISTER_H
