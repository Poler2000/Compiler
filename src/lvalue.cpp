#include "lvalue.h"

#include <utility>
#include <util.h>
#include <iostream>

LValue::LValue(std::string id)
    : Value(ValueType::TYPE_VAR), id(std::move(id)), initialized(false),
    address(0), _isMutable(true) {
}

LValue::LValue(std::string id, Value::ValueType type)
    : Value(type), id(std::move(id)), initialized(false), address(0),
    _isMutable(true) {
}

LValue::LValue(std::string id, Value::ValueType type, bool init)
    : Value(type), id(std::move(id)), initialized(init), address(0),
    _isMutable(true) {

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

bool LValue::operator==(const Value &rhs) const {
    if (rhs.get_type() == TYPE_VAR && get_type() == TYPE_VAR) {
        return id == Util::to_lvalue(rhs).id;
    }

    return false;
}
