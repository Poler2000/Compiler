#include "rvalue.h"

RValue::RValue(int value)
    : Value(ValueType::TYPE_NUMBER) {
}

bool RValue::is_initialized() const {
    return true;
}
