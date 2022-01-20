#ifndef COMPILER_V_0_1_LVALUE_H
#define COMPILER_V_0_1_LVALUE_H

#include <string>

#include "value.h"

class LValue : public Value {
public:
    explicit LValue(std::string  id);
    explicit LValue(std::string  id, ValueType type);
    explicit LValue(std::string  id, ValueType type, bool init);

    [[nodiscard]] const std::string &getId() const;
    [[nodiscard]] bool is_initialized() const override;
    [[nodiscard]] bool is_compile_time_known() const override;

    virtual uint64_t get_size();
    [[nodiscard]] uint64_t get_address() const;
    [[nodiscard]] bool isMutable() const;
    void set_mutable(bool mut);
    void set_address(uint64_t addr);
    void set_initialized(bool init);
    void set_current_value(long long value);
    void reset_current_value();
    [[nodiscard]] long long get_current_value() const;
    bool operator==(const Value &rhs) const override;

private:
    std::string id;
    bool initialized;
    uint64_t address;
    bool _isMutable;
    bool isValueKnown;
    long long currentValue;

};

#endif