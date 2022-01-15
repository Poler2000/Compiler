#include "rvalue.h"

RValue::RValue(long long value)
    : Value(ValueType::TYPE_NUMBER), value(value) {
}

bool RValue::is_initialized() const {
    return true;
}

long long RValue::get_value() const {
    return value;
}
