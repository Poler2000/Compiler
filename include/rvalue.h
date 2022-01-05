#ifndef COMPILER_V_0_1_RVALUE_H
#define COMPILER_V_0_1_RVALUE_H


#include <cstdint>
#include "value.h"

class RValue : public Value {
public:
    RValue(uint64_t value);
    [[nodiscard]] bool is_initialized() const override;
    uint64_t get_value() const;
private:
    uint64_t value;
};

#endif
