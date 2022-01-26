#include <bitset>
#include <algorithm>
#include <iostream>
#include "register.h"
#include "code_generator.h"
#include "memory_manager.h"
#include "asm.h"
#include "util.h"

void CodeGenerator::add(const Value &a, const Value& b) {
    if (a.get_type() == Value::TYPE_NUMBER && b.get_type() == Value::TYPE_NUMBER) {
        move_number_to_reg((Util::to_rvalue(a).get_value() % Util::to_rvalue(b).get_value()),
                           MemoryManager::get_a());
        return;
    }
    Register& reg = MemoryManager::get_free_reg();
    reg.free = false;

    move_value_to_reg(b, reg);
    move_value_to_reg(a, MemoryManager::get_a());

    add_line(Asm::get_instruction(Asm::ASM_ADD) + ' ' + reg.label + '\n');
    reg.free = true;
}

void CodeGenerator::sub(const Value &a, const Value& b) {
    if (a.get_type() == Value::TYPE_NUMBER && b.get_type() == Value::TYPE_NUMBER) {
        move_number_to_reg((Util::to_rvalue(a).get_value() % Util::to_rvalue(b).get_value()),
                           MemoryManager::get_a());
        return;
    }
    Register& reg = MemoryManager::get_free_reg();
    reg.free = false;
    move_value_to_reg(b, reg);
    move_value_to_reg(a, MemoryManager::get_a());

    add_line(Asm::get_instruction(Asm::ASM_SUB) + ' ' + reg.label + '\n');
    reg.free = true;
}

void CodeGenerator::mul(const Value &a, const Value& b) {
    Register& reg1 = MemoryManager::get_free_reg();
    reg1.free = false;
    Register& reg2 = MemoryManager::get_free_reg();
    reg2.free = false;
    Register& reg3 = MemoryManager::get_free_reg();
    reg3.free = false;
    move_number_to_reg(63, reg3);

    move_value_to_reg(b, reg1);
    move_value_to_reg(a, reg2);

    Register& reg4 = MemoryManager::get_free_reg();
    reg4.free = false;

    add_line(std::string(Asm::get_instruction(Asm::ASM_RESET)) + " a\n");
    blockStack.emplace(std::make_shared<CodeBlock>());
    add_line(std::string(Asm::get_instruction(Asm::ASM_SWAP)) + ' ' + reg1.label + '\n');

    copy_from_to(reg1, reg4);

    add_line(std::string(Asm::get_instruction(Asm::ASM_SWAP)) + ' ' + reg4.label + '\n');
    add_line(std::string(Asm::get_instruction(Asm::ASM_JZERO)) + " 26\n");
    //add_line(std::string(Asm::get_instruction(Asm::ASM_SHIFT)) + ' ' + reg3.label + '\n');
    // check parity
    Register& reg5 = MemoryManager::get_free_reg();
    add_line(std::string(Asm::get_instruction(Asm::ASM_SWAP)) + ' ' + reg3.label + '\n');
    add_line(std::string(Asm::get_instruction(Asm::ASM_RESET)) + " a\n");
    add_line(std::string(Asm::get_instruction(Asm::ASM_ADD)) + ' ' + reg3.label + '\n');
    add_line(std::string(Asm::get_instruction(Asm::ASM_RESET)) + ' ' + reg5.label + '\n');
    add_line(std::string(Asm::get_instruction(Asm::ASM_DEC)) + ' ' + reg5.label + '\n');
    add_line(std::string(Asm::get_instruction(Asm::ASM_SHIFT)) + ' ' + reg5.label + '\n');
    add_line(std::string(Asm::get_instruction(Asm::ASM_INC)) + ' ' + reg5.label + '\n');
    add_line(std::string(Asm::get_instruction(Asm::ASM_INC)) + ' ' + reg5.label + '\n');
    add_line(std::string(Asm::get_instruction(Asm::ASM_SHIFT)) + ' ' + reg5.label + '\n');
    add_line(std::string(Asm::get_instruction(Asm::ASM_SUB)) + ' ' + reg3.label + '\n');



    add_line(std::string(Asm::get_instruction(Asm::ASM_JZERO)) + " 4\n");

    add_line(std::string(Asm::get_instruction(Asm::ASM_SWAP)) + ' ' + reg4.label + '\n');
    add_line(std::string(Asm::get_instruction(Asm::ASM_ADD)) + ' ' + reg2.label + '\n');
    add_line(std::string(Asm::get_instruction(Asm::ASM_SWAP)) + ' ' + reg4.label + '\n');
    add_line(std::string(Asm::get_instruction(Asm::ASM_SWAP)) + ' ' + reg4.label + '\n');


    add_line(std::string(Asm::get_instruction(Asm::ASM_SWAP)) + ' ' + reg2.label + '\n');
    add_line(std::string(Asm::get_instruction(Asm::ASM_RESET)) + ' ' + reg4.label + '\n');
    add_line(std::string(Asm::get_instruction(Asm::ASM_INC)) + ' ' + reg4.label + '\n');
    add_line(std::string(Asm::get_instruction(Asm::ASM_SHIFT)) + ' ' + reg4.label + '\n');
    add_line(std::string(Asm::get_instruction(Asm::ASM_SWAP)) + ' ' + reg2.label + '\n');

    add_line(std::string(Asm::get_instruction(Asm::ASM_SWAP)) + ' ' + reg1.label + '\n');
    add_line(std::string(Asm::get_instruction(Asm::ASM_RESET)) + ' ' + reg4.label + '\n');
    add_line(std::string(Asm::get_instruction(Asm::ASM_DEC)) + ' ' + reg4.label + '\n');
    add_line(std::string(Asm::get_instruction(Asm::ASM_SHIFT)) + ' ' + reg4.label + '\n');

    auto block = blockStack.top()->get_code();
    blockStack.pop();
    reg3.free = true;
    reg4.free = true;

    add_line(std::string(Asm::get_instruction(Asm::ASM_SWAP)) + ' ' + reg2.label + "(mul begin)" + '\n');

    // a = 0 - jump to the very end
    add_line(std::string(Asm::get_instruction(Asm::ASM_JZERO)) + ' ' +
             std::to_string(4 * block.size() + 4 + 13 + 7 + 14 + 2) + '\n');
    // a > 0 - jump to + * ?
    add_line(std::string(Asm::get_instruction(Asm::ASM_JPOS)) + ' ' +
             std::to_string(2 * block.size() + 22) + '\n');
    // a < 0 - start - * ?
    // a = -a
    add_line(std::string(Asm::get_instruction(Asm::ASM_SWAP)) + ' ' + reg2.label + '\n');
    add_line(std::string(Asm::get_instruction(Asm::ASM_SUB)) + ' ' + reg2.label + '\n');
    add_line(std::string(Asm::get_instruction(Asm::ASM_RESET)) + ' ' + reg2.label + '\n');
    add_line(std::string(Asm::get_instruction(Asm::ASM_SWAP)) + ' ' + reg2.label + '\n');


    add_line(std::string(Asm::get_instruction(Asm::ASM_SWAP)) + ' ' + reg1.label + '\n');
    // b < 0 - start - * -
    add_line(std::string(Asm::get_instruction(Asm::ASM_JNEG)) + " 2\n");
    // b > 0 - jump to - * +
    add_line(std::string(Asm::get_instruction(Asm::ASM_JUMP)) + ' '
             + std::to_string(block.size() + 8) + '\n');

    // b = -b
    add_line(std::string(Asm::get_instruction(Asm::ASM_SWAP)) + ' ' + reg1.label + '\n');
    add_line(std::string(Asm::get_instruction(Asm::ASM_SUB)) + ' ' + reg1.label + '\n');
    add_line(std::string(Asm::get_instruction(Asm::ASM_RESET)) + ' ' + reg1.label + '\n');

    add_line(std::string(Asm::get_instruction(Asm::ASM_JZERO)) + ' ' +
             std::to_string(block.size() + 2) + '\n');
    std::for_each(block.begin(), block.end(), [&](auto& line){
        add_line(line);
    });
    add_line(std::string(Asm::get_instruction(Asm::ASM_JUMP)) + ' ' + std::to_string(-int(block.size() + 1)) + '\n');
    add_line(std::string(Asm::get_instruction(Asm::ASM_SWAP)) + ' ' + reg1.label + '\n');
    // jump to the very end
    add_line(std::string(Asm::get_instruction(Asm::ASM_JUMP)) + ' ' + std::to_string((3 * block.size() + 4 + 13 + 8)) + '\n');
    // end of - * -

    // start - * +
    add_line(std::string(Asm::get_instruction(Asm::ASM_JZERO)) + ' ' +
             std::to_string(block.size() + 2) + '\n');
    std::for_each(block.begin(), block.end(), [&](auto& line){
        add_line(line);
    });
    add_line(std::string(Asm::get_instruction(Asm::ASM_JUMP)) + ' ' + std::to_string(-int(block.size() + 1)) + '\n');
    add_line(std::string(Asm::get_instruction(Asm::ASM_SWAP)) + ' ' + reg1.label + '\n');
    add_line(std::string(Asm::get_instruction(Asm::ASM_RESET)) + ' ' + reg1.label + '\n');
    add_line(std::string(Asm::get_instruction(Asm::ASM_SWAP)) + ' ' + reg1.label + '\n');
    add_line(std::string(Asm::get_instruction(Asm::ASM_SUB)) + ' ' + reg1.label + '\n');
    // jump to the very end
    add_line(std::string(Asm::get_instruction(Asm::ASM_JUMP)) + ' ' + std::to_string((2 * block.size() + 4 + 14)) + '\n');
    // end of - * +

    // start of + * ?
    add_line(std::string(Asm::get_instruction(Asm::ASM_SWAP)) + ' ' + reg2.label + '\n');
    add_line(std::string(Asm::get_instruction(Asm::ASM_SWAP)) + ' ' + reg1.label + '\n');
    // b < 0 - start + * -
    add_line(std::string(Asm::get_instruction(Asm::ASM_JNEG)) + " 2\n");
    // b > 0 - jump to + * +
    add_line(std::string(Asm::get_instruction(Asm::ASM_JUMP)) + ' '
             + std::to_string(block.size() + 11) + '\n');

    // b = -b
    add_line(std::string(Asm::get_instruction(Asm::ASM_SWAP)) + ' ' + reg1.label + '\n');
    add_line(std::string(Asm::get_instruction(Asm::ASM_SUB)) + ' ' + reg1.label + '\n');
    add_line(std::string(Asm::get_instruction(Asm::ASM_RESET)) + ' ' + reg1.label + '\n');

    add_line(std::string(Asm::get_instruction(Asm::ASM_JZERO)) + ' ' +
             std::to_string(block.size() + 2) + '\n');
    std::for_each(block.begin(), block.end(), [&](auto& line){
        add_line(line);
    });
    add_line(std::string(Asm::get_instruction(Asm::ASM_JUMP)) + ' ' + std::to_string(-int(block.size() + 1)) + '\n');
    add_line(std::string(Asm::get_instruction(Asm::ASM_SWAP)) + ' ' + reg1.label + '\n');
    add_line(std::string(Asm::get_instruction(Asm::ASM_RESET)) + ' ' + reg1.label + '\n');
    add_line(std::string(Asm::get_instruction(Asm::ASM_SWAP)) + ' ' + reg1.label + '\n');
    add_line(std::string(Asm::get_instruction(Asm::ASM_SUB)) + ' ' + reg1.label + '\n');
    // jump to the very end
    add_line(std::string(Asm::get_instruction(Asm::ASM_JUMP)) + ' ' + std::to_string((block.size() + 4)) + '\n');
    // end of - * -

    // start of + * +
    add_line(std::string(Asm::get_instruction(Asm::ASM_JZERO)) + ' ' +
             std::to_string(block.size() + 2) + '\n');
    std::for_each(block.begin(), block.end(), [&](auto& line){
        add_line(line);
    });
    add_line(std::string(Asm::get_instruction(Asm::ASM_JUMP)) + ' ' + std::to_string(-int(block.size() + 1)) + '\n');
    add_line(std::string(Asm::get_instruction(Asm::ASM_SWAP)) + ' ' + reg1.label + '\n');
    // end

    reg2.free = true;
    reg1.free = true;

}

void CodeGenerator::div(const Value &a, const Value& b) {
    if (a.get_type() == Value::TYPE_NUMBER && b.get_type() == Value::TYPE_NUMBER) {
        auto aNum = Util::to_rvalue(a).get_value();
        auto bNum = Util::to_rvalue(b).get_value();
        if (aNum == 0 || bNum == 0) {
            add_line(Asm::get_instruction(Asm::ASM_RESET) + " a\n");
            return;
        }

        if (aNum * bNum > 0) {
            move_number_to_reg((aNum / bNum), MemoryManager::get_a());
            return;
        }

        if (aNum < 0) {
            long long result = (aNum - bNum + 1) / bNum;
            move_number_to_reg(result, MemoryManager::get_a());
            return;
        }

        long long result = (aNum - bNum - 1) / bNum;
        move_number_to_reg(result, MemoryManager::get_a());
        return;
    }

    Register& reg1 = MemoryManager::get_free_reg();
    reg1.free = false;
    Register& reg2 = MemoryManager::get_free_reg();
    reg2.free = false;

    // value of b should be in reg1
    move_value_to_reg(b, reg1);

    // value of a should be in reg2
    move_value_to_reg(a, reg2);

    Register& reg3 = MemoryManager::get_free_reg();
    reg3.free = false;
    Register& reg4 = MemoryManager::get_free_reg();
    reg4.free = false;
    Register& reg5 = MemoryManager::get_free_reg();
    reg5.free = false;


    blockStack.emplace(std::make_shared<CodeBlock>());

    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');

    copy_from_to(reg1, reg3);
    add_line(Asm::get_instruction(Asm::ASM_RESET) + " a\n");
    add_line(Asm::get_instruction(Asm::ASM_RESET) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_DEC) + ' ' + reg4.label + '\n');

    add_line(Asm::get_instruction(Asm::ASM_RESET) + " a\n");
    add_line(Asm::get_instruction(Asm::ASM_INC) + " a\n");

    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg3.label + " ( count 0 ) " +'\n');
    add_line(Asm::get_instruction(Asm::ASM_SUB) + ' ' + reg2.label + " ( count 0 ) " +'\n');

    add_line(Asm::get_instruction(Asm::ASM_JPOS) + " 8\n");
    add_line(Asm::get_instruction(Asm::ASM_ADD) + ' ' + reg2.label + " ( count 0 ) " +'\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg3.label + " ( count 0 ) " +'\n');

    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg3.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SHIFT) + ' ' + reg3.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg3.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_INC) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JUMP) + " -9\n");

    add_line(Asm::get_instruction(Asm::ASM_ADD) + ' ' + reg2.label + " ( count 0 ) " +'\n');
    add_line(Asm::get_instruction(Asm::ASM_RESET) + ' ' + reg3.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_DEC) + ' ' + reg3.label + " ( count 0 ) " +'\n');
    add_line(Asm::get_instruction(Asm::ASM_SHIFT) + ' ' + reg3.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_RESET) + " a\n");

    // start loop
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg4.label + " (div loop) " + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JNEG) + " 24\n");
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SUB) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JNEG) + " 9\n");

    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_RESET) + ' ' + reg5.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_INC) + ' ' + reg5.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg5.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SHIFT) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg5.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_ADD) + ' ' + reg5.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JUMP) + " 3\n");

    add_line(Asm::get_instruction(Asm::ASM_ADD) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + '\n');

    add_line(Asm::get_instruction(Asm::ASM_RESET) + ' ' + reg5.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_DEC) + ' ' + reg5.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg5.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SHIFT) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg5.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_DEC) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JUMP) + " -24\n");

    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg4.label + '\n');

    auto block = blockStack.top()->get_code();
    blockStack.pop();

    add_line(Asm::get_instruction(Asm::ASM_RESET) + " a\n");
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg5.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_RESET) + " a\n");

    // a > 0
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + " (div very begin) " +'\n');
    add_line(Asm::get_instruction(Asm::ASM_JNEG) + " 7\n");
    add_line(Asm::get_instruction(Asm::ASM_JZERO) + " 4\n");

    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JNEG) + ' ' + std::to_string((block.size() + 13)) + '\n'); // imcomptatible signs - jump
    add_line(Asm::get_instruction(Asm::ASM_JZERO) + ' ' + std::to_string((block.size() + 11)) + '\n'); // end

    // both positive
    add_line(Asm::get_instruction(Asm::ASM_JUMP) + " 10\n");

    // check b
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SUB) + ' ' + reg2.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JPOS) + ' ' + std::to_string((block.size() + 6)) + '\n'); // imcomptatible signs - jump
    add_line(Asm::get_instruction(Asm::ASM_JZERO) + ' ' + std::to_string((block.size() + 4)) + '\n'); // end
    // both negative
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_RESET) + " a\n");
    add_line(Asm::get_instruction(Asm::ASM_SUB) + ' ' + reg1.label + '\n');

    std::for_each(block.begin(), block.end(), [&](auto& line){
        add_line(line);
    });

    add_line(Asm::get_instruction(Asm::ASM_JUMP) + ' ' + std::to_string((block.size() + 23)) + '\n');


    // sign(a) != sign(b)
    add_line(Asm::get_instruction(Asm::ASM_JPOS) + " 12\n");
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_RESET) + " a\n");
    add_line(Asm::get_instruction(Asm::ASM_SUB) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_RESET) + " a\n");
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_ADD) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_DEC) + " a\n");
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');

    add_line(Asm::get_instruction(Asm::ASM_JUMP) + " 8\n");

    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_RESET) + " a\n");
    add_line(Asm::get_instruction(Asm::ASM_ADD) + ' ' + reg2.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_ADD) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_DEC) + " a\n");
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');

    std::for_each(block.begin(), block.end(), [&](auto& line){
        add_line(line);
    });

    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_RESET) + " a\n");
    add_line(Asm::get_instruction(Asm::ASM_SUB) + ' ' + reg2.label + '\n');

    reg1.free = true;
    reg2.free = true;
    reg3.free = true;
    reg4.free = true;
    reg5.free = true;
}

void CodeGenerator::mod(const Value &a, const Value& b) {
    if (a.get_type() == Value::TYPE_NUMBER && b.get_type() == Value::TYPE_NUMBER) {
        auto aNum = Util::to_rvalue(a).get_value();
        auto bNum = Util::to_rvalue(b).get_value();
        if (aNum == 0 || bNum == 0) {
            add_line(Asm::get_instruction(Asm::ASM_RESET) + " a\n");
            return;
        }

        if (aNum * bNum > 0) {
            move_number_to_reg((aNum % bNum), MemoryManager::get_a());
            return;
        }

        if (aNum < 0) {
            aNum *= -1;
            long long result = bNum - (aNum % bNum);
            move_number_to_reg(result, MemoryManager::get_a());
            return;
        }

        bNum *= -1;
        long long result = (aNum % bNum) - bNum;
        move_number_to_reg(result, MemoryManager::get_a());
        return;
    }

    Register& reg1 = MemoryManager::get_free_reg();
    reg1.free = false;
    Register& reg2 = MemoryManager::get_free_reg();
    reg2.free = false;

    // value of b should be in reg1
    move_value_to_reg(b, reg1);

    // value of a should be in reg2
    move_value_to_reg(a, reg2);

    Register& reg3 = MemoryManager::get_free_reg();
    reg3.free = false;
    Register& reg4 = MemoryManager::get_free_reg();
    reg4.free = false;
    Register& reg5 = MemoryManager::get_free_reg();
    reg5.free = false;

    blockStack.emplace(std::make_shared<CodeBlock>());

    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');

    copy_from_to(reg1, reg3);
    add_line(Asm::get_instruction(Asm::ASM_RESET) + " a\n");
    add_line(Asm::get_instruction(Asm::ASM_RESET) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_DEC) + ' ' + reg4.label + '\n');

    add_line(Asm::get_instruction(Asm::ASM_INC) + " a\n");

    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg3.label + " ( count 0 ) " +'\n');
    add_line(Asm::get_instruction(Asm::ASM_SUB) + ' ' + reg2.label + " ( count 0 ) " +'\n');

    add_line(Asm::get_instruction(Asm::ASM_JPOS) + " 8\n");
    add_line(Asm::get_instruction(Asm::ASM_ADD) + ' ' + reg2.label + " ( count 0 ) " +'\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg3.label + " ( count 0 ) " +'\n');

    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg3.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SHIFT) + ' ' + reg3.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg3.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_INC) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JUMP) + " -9\n");

    add_line(Asm::get_instruction(Asm::ASM_ADD) + ' ' + reg2.label + " ( count 0 ) " +'\n');
    add_line(Asm::get_instruction(Asm::ASM_RESET) + ' ' + reg3.label + " ( count 0 ) " +'\n');
    add_line(Asm::get_instruction(Asm::ASM_DEC) + ' ' + reg3.label + " ( count 0 ) " +'\n');
    add_line(Asm::get_instruction(Asm::ASM_SHIFT) + ' ' + reg3.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');

    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + '\n');


    // start loop
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JNEG) + " 11\n");
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SUB) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JNEG) + " 2\n");

    add_line(Asm::get_instruction(Asm::ASM_JUMP) + " 2\n");
    add_line(Asm::get_instruction(Asm::ASM_ADD) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SHIFT) + ' ' + reg3.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_DEC) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JUMP) + " -11\n");

    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg4.label + '\n');


    auto block = blockStack.top()->get_code();
    blockStack.pop();

    add_line(Asm::get_instruction(Asm::ASM_RESET) + " a\n");
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg5.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_RESET) + " a\n");

    // a > 0
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + " (mod very begin) " +'\n');
    add_line(Asm::get_instruction(Asm::ASM_JNEG) + ' ' + std::to_string((block.size() + 16)) + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JZERO) + " 4\n");

    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JPOS) + " 5\n");
    add_line(Asm::get_instruction(Asm::ASM_JZERO) + ' ' + std::to_string((block.size() + 10)) + '\n');

    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg5.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_RESET) + " a\n");
    add_line(Asm::get_instruction(Asm::ASM_SUB) + ' ' + reg5.label + '\n');

    std::for_each(block.begin(), block.end(), [&](auto& line){
        add_line(line);
    });

    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg5.label + " ( main loop end ) " + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JNEG) + " 3\n");

    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg5.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JUMP) + " 3\n");

    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg5.label + " ( main loop end ) " + '\n');
    add_line(Asm::get_instruction(Asm::ASM_ADD) + ' ' + reg5.label + '\n');


    add_line(Asm::get_instruction(Asm::ASM_JUMP) + ' ' + std::to_string((block.size() + 19)) + '\n');

    // a < 0
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SUB) + ' ' + reg2.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + '\n');

    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JPOS) + " 6\n");
    add_line(Asm::get_instruction(Asm::ASM_JZERO) + ' ' + std::to_string((block.size() + 13)) + '\n');

    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg5.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_RESET) + " a\n");
    add_line(Asm::get_instruction(Asm::ASM_SUB) + ' ' + reg5.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JUMP) + " 3\n");

    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg5.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_ADD) + ' ' + reg5.label + '\n');


    std::for_each(block.begin(), block.end(), [&](auto& line){
        add_line(line);
    });

    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg5.label + " ( main loop end ) " + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JNEG) + " 3\n");

    add_line(Asm::get_instruction(Asm::ASM_SUB) + ' ' + reg5.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JUMP) + " 3\n");

    add_line(Asm::get_instruction(Asm::ASM_RESET) + " a\n");
    add_line(Asm::get_instruction(Asm::ASM_SUB) + ' ' + reg5.label + '\n');

    reg1.free = true;
    reg2.free = true;
    reg3.free = true;
    reg4.free = true;
    reg5.free = true;
}

void CodeGenerator::write(const Value &a) {
    Register& reg = MemoryManager::get_free_reg();
    reg.free = false;
    move_value_to_reg(a, MemoryManager::get_a());

    add_line(Asm::get_instruction(Asm::Instruction::ASM_PUT) + "\n");
    reg.free = true;

}

void CodeGenerator::read(const LValue &a) {
    add_line(Asm::get_instruction(Asm::ASM_GET) + "\n");
    assign(a);
}

void CodeGenerator::assign(const Value& a) {
    Register& reg = MemoryManager::get_free_reg();
    reg.free = false;

    Register& h = MemoryManager::get_h();
    Register& g = MemoryManager::get_g();

    /*if (h.value != nullptr) {
        std::cout << "h: " << Util::to_lvalue(*h.value).getId() << ' ' << h.value->get_priority() << '\n';
        if (g.value != nullptr) {
            std::cout << "g: " << Util::to_lvalue(*g.value).getId() << ' ' << g.value->get_priority() << '\n';
        }
        if (a.get_type() == Value::TYPE_VAR) {
            std::cout << "a: " << Util::to_lvalue(a).getId() << ' ' << a.get_priority() << '\n';
        }
    }*/

    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg.label + " (assign) " + '\n');
    if (!loopStack.empty() || !branchStack.empty()) {
        if (h.value == nullptr || !(*h.value == a)) {
            if (g.value == nullptr || !(*g.value == a)) {
                move_address_to_reg(Util::to_lvalue(a), MemoryManager::get_a());
                add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg.label + '\n');
                add_line(Asm::get_instruction(Asm::ASM_STORE) + ' ' + reg.label + '\n');
            } else {
                add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg.label + " (assign) " + '\n');
                add_line(Asm::get_instruction(Asm::ASM_SWAP) + " g\n");
            }
        } else {
            add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg.label + " (assign) " + '\n');
            add_line(Asm::get_instruction(Asm::ASM_SWAP) + " h\n");
        }
        reg.free = true;

        return;
    }

    if (a.get_type() == Value::TYPE_ARRAY) {
        move_address_to_reg(Util::to_lvalue(a), MemoryManager::get_a());
        add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg.label + '\n');
        add_line(Asm::get_instruction(Asm::ASM_STORE) + ' ' + reg.label + '\n');
        reg.free = true;
        return;
    }

    if (h.value == nullptr) {
        h.value = &a;
        add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg.label + " (assign) " + '\n');
        add_line(Asm::get_instruction(Asm::ASM_SWAP) + " h\n");
        reg.free = true;
        return;
    }

    if (*h.value == a) {
        add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg.label + " (assign) " + '\n');
        add_line(Asm::get_instruction(Asm::ASM_SWAP) + " h\n");
        reg.free = true;
        return;
    }

    if (g.value == nullptr) {
        g.value = &a;
        add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg.label + " (assign) " + '\n');
        add_line(Asm::get_instruction(Asm::ASM_SWAP) + " g\n");
        reg.free = true;
        return;
    }

    if (*g.value == a) {
        add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg.label + " (assign) " + '\n');
        add_line(Asm::get_instruction(Asm::ASM_SWAP) + " g\n");
        reg.free = true;
        return;
    }


    if (h.value->get_priority() <= a.get_priority()) {
        move_address_to_reg(Util::to_lvalue(*h.value), MemoryManager::get_a());
        add_line(Asm::get_instruction(Asm::ASM_SWAP) + " h\n");
        add_line(Asm::get_instruction(Asm::ASM_STORE) + " h\n");
        add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg.label + '\n');
        add_line(Asm::get_instruction(Asm::ASM_SWAP) + " h\n");

        h.value = &a;
        reg.free = true;
        return;
    }

    if (g.value->get_priority() <= a.get_priority()) {
        move_address_to_reg(Util::to_lvalue(*g.value), MemoryManager::get_a());
        add_line(Asm::get_instruction(Asm::ASM_SWAP) + " g\n");
        add_line(Asm::get_instruction(Asm::ASM_STORE) + " g\n");
        add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg.label + '\n');
        add_line(Asm::get_instruction(Asm::ASM_SWAP) + " g\n");

        g.value = &a;
        reg.free = true;
        return;
    }

    move_address_to_reg(Util::to_lvalue(a), MemoryManager::get_a());
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_STORE) + ' ' + reg.label + '\n');

    reg.free = true;
}

std::vector<std::string> CodeGenerator::generate_asm_code() {
    return lines;
}

void CodeGenerator::eq(const Value &a, const Value &b) {
    Register& reg1 = MemoryManager::get_free_reg();
    reg1.free = false;
    Register& reg2 = MemoryManager::get_free_reg();
    reg2.free = false;

    move_value_to_reg(a, reg1);
    move_value_to_reg(b, reg2);

    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SUB) + ' ' + reg2.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JZERO) + " 2\n");

    reg1.free = true;
    reg2.free = true;
}

void CodeGenerator::neq(const Value &a, const Value &b) {
    Register& reg1 = MemoryManager::get_free_reg();
    reg1.free = false;
    Register& reg2 = MemoryManager::get_free_reg();
    reg2.free = false;

    move_value_to_reg(a, reg1);
    move_value_to_reg(b, reg2);

    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SUB) + ' ' + reg2.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JZERO) + " 2\n");
    add_line(Asm::get_instruction(Asm::ASM_JUMP) + " 2\n");

    reg1.free = true;
    reg2.free = true;
}

void CodeGenerator::le(const Value &a, const Value &b) {
    Register& reg1 = MemoryManager::get_free_reg();
    reg1.free = false;
    Register& reg2 = MemoryManager::get_free_reg();
    reg2.free = false;

    move_value_to_reg(a, reg1);
    move_value_to_reg(b, reg2);

    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SUB) + ' ' + reg2.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JNEG) + " 2\n");

    reg1.free = true;
    reg2.free = true;
}

void CodeGenerator::ge(const Value &a, const Value &b) {
    Register& reg1 = MemoryManager::get_free_reg();
    reg1.free = false;
    Register& reg2 = MemoryManager::get_free_reg();
    reg2.free = false;

    move_value_to_reg(a, reg1);
    move_value_to_reg(b, reg2);

    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SUB) + ' ' + reg2.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JPOS) + " 2\n");

    reg1.free = true;
    reg2.free = true;
}

void CodeGenerator::leq(const Value &a, const Value &b) {
    Register& reg1 = MemoryManager::get_free_reg();
    reg1.free = false;
    Register& reg2 = MemoryManager::get_free_reg();
    reg2.free = false;

    move_value_to_reg(a, reg1);
    move_value_to_reg(b, reg2);

    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SUB) + ' ' + reg2.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JPOS) + " 2\n");
    add_line(Asm::get_instruction(Asm::ASM_JUMP) + " 2\n");

    reg1.free = true;
    reg2.free = true;
}

void CodeGenerator::geq(const Value &a, const Value &b) {
    Register& reg1 = MemoryManager::get_free_reg();
    reg1.free = false;
    Register& reg2 = MemoryManager::get_free_reg();
    reg2.free = false;

    move_value_to_reg(a, reg1);
    move_value_to_reg(b, reg2);

    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SUB) + ' ' + reg2.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JNEG) + " 2\n");
    add_line(Asm::get_instruction(Asm::ASM_JUMP) + " 2\n");

    reg1.free = true;
    reg2.free = true;
}

void CodeGenerator::move_number_to_reg(const long long number, const Register &reg) {
    Register& helper = MemoryManager::get_free_reg();

    if (number >= 0 && number < 16) {
        add_line(Asm::get_instruction(Asm::ASM_RESET) + ' ' + reg.label + '\n');

        for (int i = 0; i < number; i++) {
            add_line(Asm::get_instruction(Asm::ASM_INC) + ' ' + reg.label + '\n');
        }
        return;
    }

    if (number < 0 && number > -16) {
        add_line(Asm::get_instruction(Asm::ASM_RESET) + ' ' + reg.label + '\n');

        for (int i = 0; i > number; i--) {
            add_line(Asm::get_instruction(Asm::ASM_DEC) + ' ' + reg.label + '\n');
        }
        return;
    }

    add_line(Asm::get_instruction(Asm::ASM_RESET) + " a\n");

    int i = 63;

    long long numCopy = number;

    if (number < 0) {
        numCopy *=  -1;
    }
    std::bitset<64> bits(numCopy);


    while (bits[i] == 0 && i >= 0) {
        i--;
    }
    i--;
    add_line(Asm::get_instruction(Asm::ASM_INC) + " a\n");


    add_line(Asm::get_instruction(Asm::ASM_RESET) + ' ' + helper.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_INC) + ' ' + helper.label + '\n');

    for (; i >= 0; i--) {
        add_line(Asm::get_instruction(Asm::ASM_SHIFT) + ' ' + helper.label + '\n');

        if (bits[i] == 1) {
            add_line(Asm::get_instruction(Asm::ASM_INC) + " a\n");
        }
    }

    if (number < 0) {
        add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + helper.label + '\n');
        add_line(Asm::get_instruction(Asm::ASM_RESET) + " a\n");
        add_line(Asm::get_instruction(Asm::ASM_SUB) + ' ' + helper.label + '\n');
    }

    if (reg.label != 'a') {
        add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg.label + '\n');
    }
}

void CodeGenerator::move_value_to_reg(const Value &value, const Register &reg) {
    if (value.get_type() == Value::TYPE_VAR || value.get_type() == Value::TYPE_ARRAY) {
        if (MemoryManager::get_h().value != nullptr && value.get_type() != Value::TYPE_ARRAY) {
            if (*MemoryManager::get_h().value == value) {
                copy_from_to(MemoryManager::get_h(), reg);
                return;
            }
        }
        if (MemoryManager::get_g().value != nullptr && value.get_type() != Value::TYPE_ARRAY) {
            if (*MemoryManager::get_g().value == value) {
                copy_from_to(MemoryManager::get_g(), reg);
                return;
            }
        }

        move_address_to_reg(Util::to_lvalue(value), reg);
        add_line(Asm::get_instruction(Asm::ASM_LOAD) + ' ' + reg.label + '\n');
        if (reg.label != 'a') {
            add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg.label + '\n');
        }
    } else {
        move_number_to_reg(Util::to_rvalue(value).get_value(), reg);
    }
}

void CodeGenerator::move_address_to_reg(const LValue &value, const Register &reg) {
    if (value.get_type() == Value::TYPE_VAR) {
        move_number_to_reg(value.get_address(), reg);
    } else if (value.get_type() == Value::TYPE_ARRAY) {
        Register& helper = MemoryManager::get_free_reg();
        helper.free = false;
        Register& helper2 = MemoryManager::get_free_reg();

        auto& index = Util::to_array(value).get_current();
        move_value_to_reg(index, helper);
        move_number_to_reg(value.get_address(), reg);
        if (reg.label != 'a') {
            add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg.label + '\n');
            add_line(Asm::get_instruction(Asm::ASM_ADD) + ' ' + helper.label + '\n');
            add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg.label + '\n');
        } else {
            add_line(Asm::get_instruction(Asm::ASM_ADD) + ' ' + helper.label + '\n');
            add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + helper2.label + '\n');
            helper2.free = false;
        }

        move_number_to_reg(Util::to_array(value).get_low(), helper);

        if (reg.label != 'a') {
            add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg.label + '\n');
            add_line(Asm::get_instruction(Asm::ASM_SUB) + ' ' + helper.label + '\n');
            add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg.label + '\n');
        } else {
            add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + helper2.label + '\n');
            add_line(Asm::get_instruction(Asm::ASM_SUB) + ' ' + helper.label + '\n');
        }
        helper2.free = true;
        helper.free = true;
    }
}

void CodeGenerator::start_loop(const std::shared_ptr<Loop>& loop) {
    blockStack.emplace(std::make_shared<CodeBlock>());
    loopStack.push(loop);
}

void CodeGenerator::start_for(const std::shared_ptr<Loop> &loop, const Value& from, const Value& to) {
    Register& reg1 = MemoryManager::get_free_reg();
    reg1.free = false;
    Register& reg2 = MemoryManager::get_free_reg();
    reg2.free = false;
    Register& reg3 = MemoryManager::get_free_reg();
    reg3.free = false;


    move_value_to_reg(from, reg1);
    move_value_to_reg(to, reg2);
    copy_from_to(reg1, reg3);
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');
    assign(*(loop->iterator));
    copy_from_to(reg2, reg1);
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + '\n');
    assign(*(loop->counter));
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SUB) + ' ' + reg3.label + '\n');

    blockStack.emplace(std::make_shared<CodeBlock>());
    loopStack.push(loop);
    reg1.free = true;
    reg2.free = true;
    reg3.free = true;
}

std::shared_ptr<Loop> CodeGenerator::end_for() {
    auto ptr = loopStack.top();

    //inc(*(ptr->iterator));

    move_value_to_reg(*(ptr->iterator), MemoryManager::get_a());
    add_line(Asm::get_instruction(Asm::ASM_INC) + " a\n");
    assign(*(ptr->iterator));
    move_value_to_reg(*(ptr->iterator), MemoryManager::get_a());

    Register& reg1 = MemoryManager::get_free_reg();
    reg1.free = false;


    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');
    move_value_to_reg(*(ptr->counter), MemoryManager::get_a());
    add_line(Asm::get_instruction(Asm::ASM_SUB) + ' ' + reg1.label + '\n');
    reg1.free = true;
    add_line(Asm::get_instruction(Asm::ASM_JNEG) + " 2\n");

    auto block = blockStack.top()->get_code();

    loopStack.pop();
    blockStack.pop();

    add_line(Asm::get_instruction(Asm::ASM_JNEG) + ' ' + std::to_string(block.size() + 2) + '\n');

    std::for_each(block.begin(), block.end(), [&](auto& line) {
        add_line(line);
    });
    add_line(Asm::get_instruction(Asm::ASM_JUMP) + ' ' + std::to_string(-(int)block.size()) + '\n');

    if (*MemoryManager::get_h().value == *(ptr->iterator)
        || *MemoryManager::get_h().value == *(ptr->counter)) {
        MemoryManager::get_h().value = nullptr;
    }
    if (*MemoryManager::get_g().value == *(ptr->iterator)
        || *MemoryManager::get_g().value == *(ptr->counter)) {
        MemoryManager::get_g().value = nullptr;
    }

    return ptr;
}

std::shared_ptr<Loop> CodeGenerator::end_down_to() {
    auto ptr = loopStack.top();

    move_value_to_reg(*(ptr->iterator), MemoryManager::get_a());
    add_line(Asm::get_instruction(Asm::ASM_DEC) + " a\n");
    assign(*(ptr->iterator));
    move_value_to_reg(*(ptr->iterator), MemoryManager::get_a());

    Register& reg1 = MemoryManager::get_free_reg();
    reg1.free = false;
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');
    move_value_to_reg(*(ptr->counter), MemoryManager::get_a());
    add_line(Asm::get_instruction(Asm::ASM_SUB) + ' ' + reg1.label + '\n');
    reg1.free = true;
    add_line(Asm::get_instruction(Asm::ASM_JPOS) + " 2\n");
    reg1.free = true;

    auto block = blockStack.top()->get_code();

    loopStack.pop();
    blockStack.pop();

    add_line(Asm::get_instruction(Asm::ASM_JPOS) + ' ' + std::to_string(block.size() + 2) + '\n');

    std::for_each(block.begin(), block.end(), [&](auto& line) {
        add_line(line);
    });
    add_line(Asm::get_instruction(Asm::ASM_JUMP) + ' ' + std::to_string(-(int)block.size()) + '\n');

    if (*MemoryManager::get_h().value == *(ptr->iterator)
        || *MemoryManager::get_h().value == *(ptr->counter)) {
        MemoryManager::get_h().value = nullptr;
    }
    if (*MemoryManager::get_g().value == *(ptr->iterator)
        || *MemoryManager::get_g().value == *(ptr->counter)) {
        MemoryManager::get_g().value = nullptr;
    }

    return ptr;
}

void CodeGenerator::add_line(const std::string& line) {
    if (!blockStack.empty()) {
        blockStack.top()->add_line(line);
        return;
    }
    lines.emplace_back(line);
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
    branchStack.top()->elseOffset = block->get_code().size() + 2;
    blockStack.emplace(std::make_shared<CodeBlock>());
}

void CodeGenerator::end_if() {
    auto block = blockStack.top()->get_code();

    auto branch = branchStack.top();

    branchStack.pop();
    blockStack.pop();
    if (branch->hasElse) {
        branch->elseCode = block;
        branch->endOffset += block.size() + 1;
    } else {
        branch->endOffset = block.size() + 1;
        branch->ifCode = block;
    }

    if (branch->hasElse) {
        add_line(Asm::get_instruction(Asm::ASM_JUMP) + ' ' + std::to_string(branch->elseOffset) + '\n');
    } else {
        add_line(Asm::get_instruction(Asm::ASM_JUMP) + ' ' + std::to_string(branch->endOffset) + '\n');
    }
    std::for_each(branch->ifCode.begin(), branch->ifCode.end(), [&](auto& line){
        add_line(line);
    });
    if (branch->hasElse) {
        add_line(Asm::get_instruction(Asm::ASM_JUMP) + ' ' + std::to_string(branch->elseCode.size() + 1) + '\n');
        std::for_each(block.begin(), block.end(), [&](auto& line){
            add_line(line);
        });
    }

}

std::shared_ptr<Loop> CodeGenerator::end_repeat() {
    auto ptr = loopStack.top();
    loopStack.pop();
    auto block = blockStack.top()->get_code();
    blockStack.pop();
    std::for_each(block.begin(), block.end(), [&](auto& line) {
        add_line(line);
    });
    add_line(Asm::get_instruction(Asm::ASM_JUMP) + ' ' + std::to_string(-((int)block.size())) + '\n');

    return ptr;
}

void CodeGenerator::while_cond() {
    auto block = blockStack.top()->get_code();
    blockStack.pop();
    std::for_each(block.begin(), block.end(), [&](auto& line) {
        add_line(line);
    });
    blockStack.emplace(std::make_shared<CodeBlock>());
    loopStack.top().get()->condLength = block.size();
}


std::shared_ptr<Loop> CodeGenerator::end_while() {
    auto ptr = loopStack.top();
    loopStack.pop();
    auto block = blockStack.top()->get_code();
    blockStack.pop();

    add_line(Asm::get_instruction(Asm::ASM_JUMP) + ' ' + std::to_string(block.size() + 2) + " ( while jump )" + '\n');
    std::for_each(block.begin(), block.end(), [&](auto& line) {
        add_line(line);
    });
    add_line(Asm::get_instruction(Asm::ASM_JUMP) + ' ' + std::to_string(-(int)((block.size()) + ptr->condLength + 1)) + " ( while end ) " + '\n');
    return ptr;
}

void CodeGenerator::halt() {
    if (!blockStack.empty() || !loopStack.empty() || !branchStack.empty()) {
        std::cerr << "Error: unterminated code block\n";
        exit(EXIT_FAILURE);
    }
    add_line(Asm::get_instruction(Asm::ASM_HALT));
}

void CodeGenerator::load(const Value &value) {
    move_value_to_reg(value, MemoryManager::get_a());
}

void CodeGenerator::copy_from_to(const Register &reg1, const Register &reg2) {
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_RESET) + " a\n");
    add_line(Asm::get_instruction(Asm::ASM_ADD) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + '\n');
}
