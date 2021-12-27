#ifndef COMPILER_V_0_1_LVALUE_H
#define COMPILER_V_0_1_LVALUE_H


#include "value.h"

class LValue : public Value {
public:
    std::string &getId();
};

#endif