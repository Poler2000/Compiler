#ifndef COMPILER_V_0_1_CODE_GENERATOR_H
#define COMPILER_V_0_1_CODE_GENERATOR_H

#include <vector>
#include "lvalue.h"
#include "register.h"

class CodeGenerator {
public:
    void add(const Value& a, const Value& b);
    void sub(const Value& a, const Value& b);
    void mul(const Value& a, const Value& b);
    void div(const Value& a, const Value& b);
    void mod(const Value& a, const Value& b);

    void write(const Value& a);
    void read(const LValue& a);

    void eq(const Value& a, const Value& b);
    void neq(const Value& a, const Value& b);
    void le(const Value& a, const Value& b);
    void ge(const Value& a, const Value& b);
    void leq(const Value& a, const Value& b);
    void geq(const Value& a, const Value& b);

    std::string generate_asm_code();
private:
    std::vector<std::string> lines;

    void move_address_to_reg(const LValue &value, const Register &reg);
    void move_number_to_reg(const uint64_t number, const Register &reg);
};


#endif //COMPILER_V_0_1_CODE_GENERATOR_H
