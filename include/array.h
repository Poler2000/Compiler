#ifndef COMPILER_V_0_1_ARRAY_H
#define COMPILER_V_0_1_ARRAY_H

#include "lvalue.h"

class Array : public LValue {
public:
    uint64_t get_size() override;
    void set_current(Value* val);
private:
    long firstIndex;
    long lastIndex;
};


#endif //COMPILER_V_0_1_ARRAY_H
