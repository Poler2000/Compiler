#include <util.h>
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

bool RValue::operator==(const Value &rhs) const {
    if (rhs.get_type() == TYPE_NUMBER) {
        return value == Util::to_rvalue(rhs).value;
    }

    return false;
}
