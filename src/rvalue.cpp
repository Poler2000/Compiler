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

bool RValue::is_compile_time_known() const {
    return true;
}

bool RValue::operator==(const Value &rhs) const {
    if (!rhs.is_compile_time_known()) {
        return false;
    } else {
        if (rhs.get_type() == TYPE_NUMBER) {
            return value == Util::to_rvalue(rhs).value;
        } else {
            return value == Util::to_lvalue(rhs).get_current_value();
        }
    }
}
