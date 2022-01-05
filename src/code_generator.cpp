#include <bitset>
#include "register.h"
#include "code_generator.h"
#include "memory_manager.h"
#include "asm.h"
#include "util.h"

void CodeGenerator::add(const Value &a, const Value& b) {

}

void CodeGenerator::sub(const Value &a, const Value& b) {

}

void CodeGenerator::mul(const Value &a, const Value& b) {

}

void CodeGenerator::div(const Value &a, const Value& b) {

}

void CodeGenerator::mod(const Value &a, const Value& b) {

}

void CodeGenerator::write(const Value &a) {
    Register& reg = MemoryManager::get_a();
    if (a.get_type() == Value::TYPE_NUMBER) {

    } else {
        move_address_to_reg(Util::to_lvalue(a), reg);
    }

    lines.emplace_back(std::string(Asm::get_instruction(Asm::Instruction::ASM_PUT)) + "\n");
}

void CodeGenerator::read(const LValue &a) {
    lines.emplace_back(std::string(Asm::get_instruction(Asm::ASM_GET)) + '\n');
    Register& addrReg = MemoryManager::get_free_reg();
    addrReg.free = false;
    move_address_to_reg(a, addrReg);
    lines.emplace_back(std::string(Asm::get_instruction(Asm::ASM_STORE)) + ' ' + addrReg.label + '\n');
    addrReg.free = true;
}

void CodeGenerator::assign(const LValue& a) {
    Register& reg = MemoryManager::get_free_reg();
    move_address_to_reg(a, reg);
    lines.emplace_back(std::string(Asm::get_instruction(Asm::ASM_STORE)) + ' ' + reg.label + '\n');
}

std::string CodeGenerator::generate_asm_code() {
    return std::string();
}

void CodeGenerator::eq(const Value &a, const Value &b) {

}

void CodeGenerator::neq(const Value &a, const Value &b) {

}

void CodeGenerator::le(const Value &a, const Value &b) {

}

void CodeGenerator::ge(const Value &a, const Value &b) {

}

void CodeGenerator::leq(const Value &a, const Value &b) {

}

void CodeGenerator::geq(const Value &a, const Value &b) {

}

void CodeGenerator::move_number_to_reg(const uint64_t number, const Register &reg) {
    Register& helper = MemoryManager::get_free_reg();
    lines.emplace_back(std::string(Asm::get_instruction(Asm::ASM_RESET)) + " a\n");
    lines.emplace_back(std::string(Asm::get_instruction(Asm::ASM_RESET)) + ' ' + helper.label + '\n');
    lines.emplace_back(std::string(Asm::get_instruction(Asm::ASM_INC)) + ' ' + helper.label + '\n');
    std::bitset<64> bits(number);
    Register& addrReg = MemoryManager::get_free_reg();
    for (int i = 0; i < bits.size(); i++) {
        lines.emplace_back(std::string(Asm::get_instruction(Asm::ASM_SHIFT)) + ' ' + helper.label + '\n');
        if (bits[i]) {
            lines.emplace_back(std::string(Asm::get_instruction(Asm::ASM_INC)) + " a\n");
        }
    }
    lines.emplace_back(std::string(Asm::get_instruction(Asm::ASM_SWAP)) + ' ' + reg.label + '\n');
}

void CodeGenerator::move_address_to_reg(const LValue &value, const Register &reg) {
    if (value.get_type() == Value::TYPE_VAR) {
        move_number_to_reg(value.get_address(), reg);
    }
}

void CodeGenerator::start_loop(const std::shared_ptr<Loop>& loop) {
    loopStack.push(loop);
}

std::shared_ptr<Loop> CodeGenerator::end_loop() {
    auto ptr = loopStack.top();
    loopStack.pop();
    return ptr;
}
