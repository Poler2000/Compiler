#include "lvalue.h"

LValue::LValue(const std::string &id)
    : Value(ValueType::TYPE_VAR), id(id), initialized(false) {
}

LValue::LValue(const std::string &id, Value::ValueType type)
    : Value(type), id(id), initialized(false) {
}

LValue::LValue(const std::string &id, Value::ValueType type, bool init)
    : Value(type), id(id), initialized(init){

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
