#include "lvalue.h"

#include <utility>
#include <util.h>
#include <iostream>

LValue::LValue(std::string id)
    : Value(ValueType::TYPE_VAR), id(std::move(id)), initialized(false),
    address(0), _isMutable(true), isValueKnown(false), currentValue(0) {
}

LValue::LValue(std::string id, Value::ValueType type)
    : Value(type), id(std::move(id)), initialized(false), address(0),
    _isMutable(true), isValueKnown(false), currentValue(0) {
}

LValue::LValue(std::string id, Value::ValueType type, bool init)
    : Value(type), id(std::move(id)), initialized(init), address(0),
    _isMutable(true), isValueKnown(false), currentValue(0) {

}

const std::string &LValue::getId() const {
    return id;
}

bool LValue::is_initialized() const {
    return initialized;
}

uint64_t LValue::get_size() {
    return 1;
}

void LValue::set_address(const uint64_t addr) {
    address = addr;
}

uint64_t LValue::get_address() const {
    return address;
}

bool LValue::isMutable() const {
    return _isMutable;
}

void LValue::set_mutable(bool mut) {
    _isMutable = mut;
}

void LValue::set_initialized(bool init) {
    initialized = init;
}

bool LValue::is_compile_time_known() const {
    return isValueKnown;
}

void LValue::set_current_value(long long int value) {
    currentValue = value;
    isValueKnown = true;
}

void LValue::reset_current_value() {
    isValueKnown = false;
}

long long LValue::get_current_value() const {
    return currentValue;
}

bool LValue::operator==(const Value &rhs) const {
    //std::cout << "I perform this\n";

    if (!rhs.is_compile_time_known() || !isValueKnown) {
        return id == Util::to_lvalue(rhs).id;
    } else {
        if (rhs.get_type() == TYPE_NUMBER) {
            return currentValue == Util::to_rvalue(rhs).get_value();
        } else {
            return currentValue == Util::to_lvalue(rhs).get_current_value();
        }
    }
}
