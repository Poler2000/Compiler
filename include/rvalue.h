#ifndef COMPILER_V_0_1_RVALUE_H
#define COMPILER_V_0_1_RVALUE_H


#include <cstdint>
#include "value.h"

class RValue : public Value {
public:
    RValue(long long value);
    [[nodiscard]] bool is_initialized() const override;
    [[nodiscard]] bool is_compile_time_known() const override;
    [[nodiscard]] long long get_value() const;
    bool operator==(const Value &rhs) const override;

private:
    long long value;
};

#endif
