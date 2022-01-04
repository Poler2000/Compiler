#ifndef COMPILER_V_0_1_LVALUE_H
#define COMPILER_V_0_1_LVALUE_H

#include <string>

#include "value.h"

class LValue : public Value {
public:
    explicit LValue(const std::string& id);
    explicit LValue(const std::string& id, ValueType type);
    explicit LValue(const std::string& id, ValueType type, bool init);
    [[nodiscard]] const std::string &getId() const;
    [[nodiscard]] bool is_initialized() const override;
    virtual uint64_t get_size();
    uint64_t get_address() const;

    void set_address(uint64_t addr);

private:
    std::string id;
    bool initialized;
    uint64_t address;
};

#endif