#include <iostream>

#include "array.h"

uint64_t Array::get_size() {
    return lastIndex - firstIndex + 1;
}

void Array::set_current(Value *val) {
    current = val;
}

Array::Array(const std::string& id, long firstIndex, long lastIndex)
    : LValue(id, ValueType::TYPE_ARRAY, true), firstIndex(firstIndex), lastIndex(lastIndex) {
}

const Value &Array::get_current() const {
    return *current;
}

long Array::get_low() const {
    std::cout << "Retirning " << firstIndex << '\n';
    return firstIndex;
}
