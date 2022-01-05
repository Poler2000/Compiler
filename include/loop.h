#ifndef COMPILER_V_0_1_LOOP_H
#define COMPILER_V_0_1_LOOP_H

#include "lvalue.h"

struct Loop {
    LValue* iterator;
    LValue* counter;
    /*std::string l1;
    std::string l2;*/
    uint64_t start;
    uint64_t end;
};

#endif //COMPILER_V_0_1_LOOP_H
