#ifndef COMPILER_V_0_1_ARRAY_H
#define COMPILER_V_0_1_ARRAY_H

#include <memory>
#include <vector>
#include "lvalue.h"

class Array : public LValue {
public:
    Array(const std::string& id, long firstIndex, long lastIndex);
    uint64_t get_size() override;
    void set_current(Value* val);
    long get_low() const;
    const Value& get_current() const;
private:
    std::vector<std::shared_ptr<LValue>> elements;
    Value* current;
    long firstIndex;
    long lastIndex;
};


#endif //COMPILER_V_0_1_ARRAY_H
