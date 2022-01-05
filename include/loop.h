#ifndef COMPILER_V_0_1_LOOP_H
#define COMPILER_V_0_1_LOOP_H

#include "lvalue.h"

struct Loop {
    LValue* iterator;
    LValue* counter;
};

#endif //COMPILER_V_0_1_LOOP_H
