#ifndef COMPILER_V_0_1_UTIL_H
#define COMPILER_V_0_1_UTIL_H

#include "lvalue.h"
#include "rvalue.h"

class Util {
public:
    static const LValue& to_lvalue(const Value& val);
    static LValue& to_mut_lvalue(Value& val);
    static const RValue& to_rvalue(const Value& val);
};

#endif //COMPILER_V_0_1_UTIL_H
