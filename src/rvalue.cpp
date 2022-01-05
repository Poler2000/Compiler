#include "rvalue.h"

RValue::RValue(uint64_t value)
    : Value(ValueType::TYPE_NUMBER), value(value) {
}

bool RValue::is_initialized() const {
    return true;
}

uint64_t RValue::get_value() const {
    return value;
}
