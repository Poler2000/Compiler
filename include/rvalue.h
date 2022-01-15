#ifndef COMPILER_V_0_1_RVALUE_H
#define COMPILER_V_0_1_RVALUE_H


#include <cstdint>
#include "value.h"

class RValue : public Value {
public:
    RValue(long long value);
    [[nodiscard]] bool is_initialized() const override;
    long long get_value() const;
private:
    long long value;
};

#endif
