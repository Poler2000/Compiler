#include <bitset>
#include <algorithm>
#include <iostream>
#include "register.h"
#include "code_generator.h"
#include "memory_manager.h"
#include "asm.h"
#include "util.h"

void CodeGenerator::add(const Value &a, const Value& b) {
    Register& reg = MemoryManager::get_free_reg();
    std::cout << "1\n";
    if (b.get_type() == Value::TYPE_NUMBER) {
        move_number_to_reg(Util::to_rvalue(b).get_value(), reg);
    } else {
        move_address_to_reg(Util::to_lvalue(b), reg);
        add_line(std::string(Asm::get_instruction(Asm::ASM_LOAD)) + ' ' + reg.label + '\n');
        add_line(std::string(Asm::get_instruction(Asm::ASM_SWAP)) + ' ' + reg.label + '\n');
    }
    std::cout << "2\n";

    if (a.get_type() == Value::TYPE_NUMBER) {
        move_number_to_a(Util::to_rvalue(a).get_value());
    } else {
        move_number_to_a(Util::to_lvalue(a).get_address());
        add_line(std::string(Asm::get_instruction(Asm::ASM_LOAD)) + ' ' + reg.label + '\n');
        add_line(std::string(Asm::get_instruction(Asm::ASM_SWAP)) + ' ' + reg.label + '\n');
    }
    add_line(std::string(Asm::get_instruction(Asm::ASM_ADD)) + ' ' + reg.label + '\n');
}

void CodeGenerator::sub(const Value &a, const Value& b) {
    Register& reg = MemoryManager::get_free_reg();

    if (b.get_type() == Value::TYPE_NUMBER) {
        move_number_to_reg(Util::to_rvalue(b).get_value(), reg);
    } else {
        move_address_to_reg(Util::to_lvalue(b), reg);
        add_line(std::string(Asm::get_instruction(Asm::ASM_LOAD)) + ' ' + reg.label + '\n');
        add_line(std::string(Asm::get_instruction(Asm::ASM_SWAP)) + ' ' + reg.label + '\n');
    }

    if (a.get_type() == Value::TYPE_NUMBER) {
        move_number_to_a(Util::to_rvalue(b).get_value());
    } else {
        move_number_to_a(Util::to_lvalue(a).get_address());
        add_line(std::string(Asm::get_instruction(Asm::ASM_LOAD)) + ' ' + reg.label + '\n');
        add_line(std::string(Asm::get_instruction(Asm::ASM_SWAP)) + ' ' + reg.label + '\n');
    }
    add_line(std::string(Asm::get_instruction(Asm::ASM_SUB)) + ' ' + reg.label + '\n');
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

    add_line(std::string(Asm::get_instruction(Asm::Instruction::ASM_PUT)) + "\n");
}

void CodeGenerator::read(const LValue &a) {
    add_line(std::string(Asm::get_instruction(Asm::ASM_GET)) + "\n");

    Register& addrReg = MemoryManager::get_free_reg();

    addrReg.free = false;

    move_address_to_reg(a, addrReg);
    add_line(std::string(Asm::get_instruction(Asm::ASM_STORE)) + ' ' + addrReg.label + '\n');
    addrReg.free = true;
}

void CodeGenerator::assign(const LValue& a) {
    Register& reg = MemoryManager::get_free_reg();
    reg.free = false;
    Register& reg2 = MemoryManager::get_free_reg();
    reg2.free = false;
    add_line(std::string(Asm::get_instruction(Asm::ASM_SWAP)) + ' ' + reg2.label + '\n');

    move_address_to_reg(a, reg);
    add_line(std::string(Asm::get_instruction(Asm::ASM_SWAP)) + ' ' + reg2.label + '\n');
    reg2.free = true;

    add_line(std::string(Asm::get_instruction(Asm::ASM_STORE)) + ' ' + reg.label + '\n');
    reg.free = true;
}

std::vector<std::string> CodeGenerator::generate_asm_code() {
    return lines;
}

void CodeGenerator::eq(const Value &a, const Value &b) {
    //blockStack.emplace(std::make_shared<CodeBlock>());
    Register& reg1 = MemoryManager::get_free_reg();
    Register& reg2 = MemoryManager::get_free_reg();

    if (a.get_type() == Value::TYPE_VAR) {
        if (b.get_type() == Value::TYPE_VAR) {
            move_address_to_reg(Util::to_lvalue(b), reg2);
            add_line(std::string(Asm::get_instruction(Asm::ASM_LOAD)) + ' ' + reg2.label + '\n');

            add_line(std::string(Asm::get_instruction(Asm::ASM_SWAP)) + ' ' + reg2.label + '\n');

        } else if (b.get_type() == Value::TYPE_NUMBER) {
            move_number_to_reg(Util::to_rvalue(b).get_value(), reg2);
        } else {
            exit(EXIT_FAILURE);
        }

        move_address_to_reg(Util::to_lvalue(a), reg1);
        add_line(std::string(Asm::get_instruction(Asm::ASM_LOAD)) + ' ' + reg1.label + '\n');
        add_line(std::string(Asm::get_instruction(Asm::ASM_SUB)) + ' ' + reg2.label + '\n');
    } else if (a.get_type() == Value::TYPE_NUMBER) {
        move_number_to_reg(Util::to_rvalue(a).get_value(), reg1);

        if (b.get_type() == Value::TYPE_VAR) {
            move_address_to_reg(Util::to_lvalue(b), reg2);
            add_line(std::string(Asm::get_instruction(Asm::ASM_LOAD)) + ' ' + reg2.label + '\n');

        } else if (b.get_type() == Value::TYPE_NUMBER) {
            move_number_to_reg(Util::to_rvalue(b).get_value(), reg2);
        } else {
            exit(EXIT_FAILURE);
        }
    } else {
        exit(EXIT_FAILURE);
    }






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
    add_line(std::string(Asm::get_instruction(Asm::ASM_RESET)) + " a\n");
    add_line(std::string(Asm::get_instruction(Asm::ASM_RESET)) + ' ' + helper.label + '\n');
    add_line(std::string(Asm::get_instruction(Asm::ASM_INC)) + ' ' + helper.label + '\n');
    std::bitset<64> bits(number);
    Register& addrReg = MemoryManager::get_free_reg();
    int i = 63;
    std::cout << bits << '\n';

    while (bits[i] == 0 && i >= 0) {
        i--;
        std::cout << bits[i] << ' ';
    }
    for (; i >= 0; i--) {
        std::cout << bits[i] << ' ';
        add_line(std::string(Asm::get_instruction(Asm::ASM_SHIFT)) + ' ' + helper.label + '\n');
        if (bits[i] == 1) {
            add_line(std::string(Asm::get_instruction(Asm::ASM_INC)) + " a\n");
        }
    }
    std::cout << '\n';

    add_line(std::string(Asm::get_instruction(Asm::ASM_SWAP)) + ' ' + reg.label + '\n');
}

void CodeGenerator::move_address_to_reg(const LValue &value, const Register &reg) {
    if (value.get_type() == Value::TYPE_VAR) {
        move_number_to_reg(value.get_address(), reg);
    }
}

void CodeGenerator::start_loop(const std::shared_ptr<Loop>& loop) {
    blockStack.emplace(std::make_shared<CodeBlock>());
    loopStack.push(loop);
}

std::shared_ptr<Loop> CodeGenerator::end_for() {
    auto ptr = loopStack.top();
    loopStack.pop();

    inc(*(ptr->counter));
    auto block = blockStack.top();
    blockStack.pop();
    add_line(std::string(Asm::get_instruction(Asm::ASM_JUMP)) + ' ' + std::to_string(-block->get_code().size() - 1) + '\n');
    return ptr;
}

void CodeGenerator::add_line(const std::string& line) {
    if (!blockStack.empty()) {
        blockStack.top()->add_line(line);
        return;
    }
    lines.emplace_back(line);
}

void CodeGenerator::inc(const LValue& value) {
    Register& reg = MemoryManager::get_free_reg();
    move_address_to_reg(value, reg);
    add_line(std::string(Asm::get_instruction(Asm::ASM_LOAD)) + ' ' + reg.label + '\n');
    add_line(std::string(Asm::get_instruction(Asm::ASM_INC)) + " a\n");
    add_line(std::string(Asm::get_instruction(Asm::ASM_STORE)) + ' ' + reg.label + '\n');
}

void CodeGenerator::move_variable_to_reg(const LValue &value, Register &reg) {

}

void CodeGenerator::start_if() {
    branchStack.emplace(std::make_shared<Branch>());
    blockStack.emplace(std::make_shared<CodeBlock>());
}

void CodeGenerator::start_else() {
    auto block = blockStack.top();
    blockStack.pop();
    branchStack.top()->hasElse = true;
    branchStack.top()->ifCode = block->get_code();
    branchStack.top()->elseOffset = block->get_code().size() + 1;
    blockStack.emplace(std::make_shared<CodeBlock>());
}

void CodeGenerator::end_if() {
    auto block = blockStack.top()->get_code();

    auto branch = branchStack.top();

    branchStack.pop();

    blockStack.pop();
    if (branch->hasElse) {
        branch->elseOffset = branch->ifCode.size() + 1;

        branch->elseCode = block;
        branch->endOffset += block.size() + 1;
    } else {
        branch->endOffset = block.size() + 1;
        branch->ifCode = block;
    }

    add_line(std::string(Asm::get_instruction(Asm::ASM_JZERO)) + ' ' + std::to_string(branch->endOffset) + '\n');
    std::for_each(branch->ifCode.begin(), branch->ifCode.end(), [&](auto& line){
        add_line(line);
    });
    if (branch->hasElse) {
        add_line(std::string(Asm::get_instruction(Asm::ASM_JUMP)) + ' ' + std::to_string(branch->elseCode.size() + 1) + '\n');
        std::for_each(block.begin(), block.end(), [&](auto& line){
            add_line(line);
        });
    }

}

std::shared_ptr<Loop> CodeGenerator::end_repeat() {
    std::cout << "0\n";

    auto ptr = loopStack.top();
    loopStack.pop();
    auto block = blockStack.top()->get_code();
    blockStack.pop();
    std::cout << "3\n";
    std::for_each(block.begin(), block.end(), [&](auto& line) {
        add_line(line);
    });
    add_line(std::string(Asm::get_instruction(Asm::ASM_JUMP)) + ' ' + std::to_string(-((int)block.size()) - 1) + '\n');
    std::cout << "4\n";

    return ptr;
}

void CodeGenerator::move_number_to_a(uint64_t value) {
    Register& helper = MemoryManager::get_free_reg();
    add_line(std::string(Asm::get_instruction(Asm::ASM_RESET)) + " a\n");
    add_line(std::string(Asm::get_instruction(Asm::ASM_RESET)) + ' ' + helper.label + '\n');
    add_line(std::string(Asm::get_instruction(Asm::ASM_INC)) + ' ' + helper.label + '\n');
    std::bitset<64> bits(value);
    int i = 63;

    while (bits[i] == 0 && i >= 0) {
        i--;
    }
    for (; i >= 0; i--) {
        add_line(std::string(Asm::get_instruction(Asm::ASM_SHIFT)) + ' ' + helper.label + '\n');
        if (bits[i] == 1) {
            add_line(std::string(Asm::get_instruction(Asm::ASM_INC)) + " a\n");
        }
    }
}
