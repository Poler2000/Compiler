#ifndef COMPILER_V_0_1_CODE_GENERATOR_H
#define COMPILER_V_0_1_CODE_GENERATOR_H

#include <vector>
#include <stack>
#include <memory>
#include "lvalue.h"
#include "register.h"
#include "loop.h"
#include "code_block.h"
#include "branch.h"

class CodeGenerator {
public:
    void add(const Value& a, const Value& b);
    void sub(const Value& a, const Value& b);
    void mul(const Value& a, const Value& b);
    void div(const Value& a, const Value& b);
    void mod(const Value& a, const Value& b);
    void mod(const Value& a, const Value& b, const Value& helper);

    void write(const Value& a);
    void read(const LValue& a);
    void assign(const Value& a);

    void eq(const Value& a, const Value& b);
    void neq(const Value& a, const Value& b);
    void le(const Value& a, const Value& b);
    void ge(const Value& a, const Value& b);
    void leq(const Value& a, const Value& b);
    void geq(const Value& a, const Value& b);

    void start_loop(const std::shared_ptr<Loop>& loop);
    void start_for(const std::shared_ptr<Loop>& loop, const Value& from, const Value& to);
    std::shared_ptr<Loop> end_for();
    std::shared_ptr<Loop> end_down_to();
    std::shared_ptr<Loop> end_repeat();
    std::shared_ptr<Loop> end_while();
    void while_cond();

    void start_if();
    void start_else();
    void end_if();

    void load(const Value &value);

    std::vector<std::string> generate_asm_code();

    void halt();

private:
    std::vector<std::string> lines;
    std::stack<std::shared_ptr<Loop>> loopStack;
    std::stack<std::shared_ptr<CodeBlock>> blockStack;
    std::stack<std::shared_ptr<Branch>> branchStack;

    void move_address_to_reg(const LValue &value, const Register &reg);
    void move_number_to_reg(const long long number, const Register &reg);
    void move_value_to_reg(const Value &value, const Register &reg);

    void add_line(const std::string& line);

    void inc(const LValue& value);

    void move_number_to_a(long long value);

    void copy_from_to(Register &reg1, Register &reg2);

    void dec(const LValue &value);
};


#endif //COMPILER_V_0_1_CODE_GENERATOR_H
