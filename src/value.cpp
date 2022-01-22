#include "value.h"

Value::Value(Value::ValueType type)
    :type(type) {

}

Value::ValueType Value::get_type() const {
    return type;
}

bool Value::is_initialized() const {
    return false;
}

void Value::inc_priority() {
    priority++;
}

long long Value::get_priority() const {
    return priority;
}
