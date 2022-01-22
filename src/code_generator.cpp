#include <bitset>
#include <algorithm>
#include <iostream>
#include "register.h"
#include "code_generator.h"
#include "memory_manager.h"
#include "asm.h"
#include "util.h"

void CodeGenerator::add(const Value &a, const Value& b) {
    Register& reg = MemoryManager::get_free_reg(this);
    reg.free = false;

    move_value_to_reg(b, reg);
    move_value_to_reg(a, MemoryManager::get_a());

    add_line(Asm::get_instruction(Asm::ASM_ADD) + ' ' + reg.label + '\n');
    reg.free = true;
}

void CodeGenerator::sub(const Value &a, const Value& b) {
    Register& reg = MemoryManager::get_free_reg(this);
    reg.free = false;
    move_value_to_reg(b, reg);
    move_value_to_reg(a, MemoryManager::get_a());

    add_line(Asm::get_instruction(Asm::ASM_SUB) + ' ' + reg.label + '\n');
    reg.free = true;
}

void CodeGenerator::mul(const Value &a, const Value& b) {
    Register& reg1 = MemoryManager::get_free_reg(this);
    reg1.free = false;
    Register& reg2 = MemoryManager::get_free_reg(this);
    reg2.free = false;
    Register& reg3 = MemoryManager::get_free_reg(this);
    reg3.free = false;
    Register& reg4 = MemoryManager::get_free_reg(this);
    reg4.free = false;
    move_number_to_reg(63, reg3);

    // value of b should be in reg1
    move_value_to_reg(b, reg1);
    // value of a should be in reg2
    move_value_to_reg(a, reg2);

    Register& reg5 = MemoryManager::get_free_reg(this);
    reg5.free = false;
    Register& reg6 = MemoryManager::get_free_reg(this);
    reg6.free = false;
    add_line(Asm::get_instruction(Asm::ASM_RESET) + ' ' + reg5.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_DEC) + ' ' + reg5.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_RESET) + ' ' + reg6.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_INC) + ' ' + reg6.label + '\n');

    add_line(Asm::get_instruction(Asm::ASM_RESET) + " a\n");
    blockStack.emplace(std::make_shared<CodeBlock>());
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');

    copy_from_to(reg1, reg4);

    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JZERO) + " 22\n");
    // check parity
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg3.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_RESET) + " a\n");
    add_line(Asm::get_instruction(Asm::ASM_ADD) + ' ' + reg3.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SHIFT) + ' ' + reg5.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SHIFT) + ' ' + reg6.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SUB) + ' ' + reg3.label + '\n');

    add_line(Asm::get_instruction(Asm::ASM_JZERO) + " 4\n");

    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_ADD) + ' ' + reg2.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg4.label + '\n');


    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_RESET) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_INC) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SHIFT) + ' ' + reg4.label + '\n');
    //add_line(Asm::get_instruction(Asm::ASM_PUT) + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + '\n');

    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_RESET) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_DEC) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SHIFT) + ' ' + reg4.label + '\n');
    //add_line(Asm::get_instruction(Asm::ASM_PUT) + '\n');

    auto block = blockStack.top()->get_code();
    blockStack.pop();
    reg3.free = true;
    reg4.free = true;
    reg5.free = true;
    reg6.free = true;

    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + "(mul begin)" + '\n');

    // a = 0 - jump to the very end
    add_line(Asm::get_instruction(Asm::ASM_JZERO) + ' ' +
        std::to_string(4 * block.size() + 4 + 13 + 7 + 14 + 2) + '\n');
    // a > 0 - jump to + * ?
    add_line(Asm::get_instruction(Asm::ASM_JPOS) + ' ' +
             std::to_string(2 * block.size() + 22) + '\n');
    // a < 0 - start - * ?
    // a = -a
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SUB) + ' ' + reg2.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_RESET) + ' ' + reg2.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + '\n');


    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');
    // b < 0 - start - * -
    add_line(Asm::get_instruction(Asm::ASM_JNEG) + " 2\n");
    // b > 0 - jump to - * +
    add_line(Asm::get_instruction(Asm::ASM_JUMP) + ' '
        + std::to_string(block.size() + 8) + '\n');

    // b = -b
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SUB) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_RESET) + ' ' + reg1.label + '\n');

    add_line(Asm::get_instruction(Asm::ASM_JZERO) + ' ' +
             std::to_string(block.size() + 2) + '\n');
    std::for_each(block.begin(), block.end(), [&](auto& line){
        add_line(line);
    });
    add_line(Asm::get_instruction(Asm::ASM_JUMP) + ' ' + std::to_string(-int(block.size() + 1)) + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');
    // jump to the very end
    add_line(Asm::get_instruction(Asm::ASM_JUMP) + ' ' + std::to_string((3 * block.size() + 4 + 13 + 8)) + '\n');
    // end of - * -

    // start - * +
    add_line(Asm::get_instruction(Asm::ASM_JZERO) + ' ' +
             std::to_string(block.size() + 2) + '\n');
    std::for_each(block.begin(), block.end(), [&](auto& line){
        add_line(line);
    });
    add_line(Asm::get_instruction(Asm::ASM_JUMP) + ' ' + std::to_string(-int(block.size() + 1)) + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_RESET) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SUB) + ' ' + reg1.label + '\n');
    // jump to the very end
    add_line(Asm::get_instruction(Asm::ASM_JUMP) + ' ' + std::to_string((2 * block.size() + 4 + 14)) + '\n');
    // end of - * +

    // start of + * ?
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');
    // b < 0 - start + * -
    add_line(Asm::get_instruction(Asm::ASM_JNEG) + " 2\n");
    // b > 0 - jump to + * +
    add_line(Asm::get_instruction(Asm::ASM_JUMP) + ' '
             + std::to_string(block.size() + 11) + '\n');

    // b = -b
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SUB) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_RESET) + ' ' + reg1.label + '\n');

    add_line(Asm::get_instruction(Asm::ASM_JZERO) + ' ' +
             std::to_string(block.size() + 2) + '\n');
    std::for_each(block.begin(), block.end(), [&](auto& line){
        add_line(line);
    });
    add_line(Asm::get_instruction(Asm::ASM_JUMP) + ' ' + std::to_string(-int(block.size() + 1)) + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_RESET) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SUB) + ' ' + reg1.label + '\n');
    // jump to the very end
    add_line(Asm::get_instruction(Asm::ASM_JUMP) + ' ' + std::to_string((block.size() + 4)) + '\n');
    // end of - * -

    // start of + * +
    add_line(Asm::get_instruction(Asm::ASM_JZERO) + ' ' +
             std::to_string(block.size() + 2) + '\n');
    std::for_each(block.begin(), block.end(), [&](auto& line){
        add_line(line);
    });
    add_line(Asm::get_instruction(Asm::ASM_JUMP) + ' ' + std::to_string(-int(block.size() + 1)) + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');
    // end

    reg2.free = true;
    reg1.free = true;


}

void CodeGenerator::div(const Value &a, const Value& b) {
    Register& reg1 = MemoryManager::get_free_reg(this);
    reg1.free = false;
    Register& reg2 = MemoryManager::get_free_reg(this);
    reg2.free = false;
    Register& reg3 = MemoryManager::get_free_reg(this);
    reg3.free = false;
    Register& reg4 = MemoryManager::get_free_reg(this);
    reg4.free = false;

    // value of b should be in reg1
    move_value_to_reg(b, reg1);

    // value of a should be in reg2
    move_value_to_reg(a, reg2);

    Register& reg5 = MemoryManager::get_free_reg(this);
    reg5.free = false;

    blockStack.emplace(std::make_shared<CodeBlock>());

    add_line(Asm::get_instruction(Asm::ASM_RESET) + " a  (div) \n");
    copy_from_to(reg1, reg4);

    copy_from_to(reg2, reg3);

    add_line(Asm::get_instruction(Asm::ASM_RESET) + ' ' + reg5.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_INC) + ' ' + reg5.label + '\n');

    // count 0
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg4.label + " ( count 0 ) " +'\n');
    add_line(Asm::get_instruction(Asm::ASM_JZERO) + " 11\n");
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_RESET) + " a\n");
    add_line(Asm::get_instruction(Asm::ASM_DEC) + " a\n");

    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SHIFT) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg3.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SHIFT) + ' ' + reg3.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg3.label + '\n');

    add_line(Asm::get_instruction(Asm::ASM_JUMP) + " -11\n");

    // reload b
    copy_from_to(reg1, reg4);

    Register& reg6  = MemoryManager::get_free_reg(this);

    // add 0
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg3.label + " ( add 0 ) " + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JZERO) + " 12\n");
    add_line(Asm::get_instruction(Asm::ASM_RESET) + ' ' + reg6.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_DEC) + ' ' + reg6.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SHIFT) + ' ' + reg6.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg3.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_INC) + ' ' + reg6.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_INC) + ' ' + reg6.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SHIFT) + ' ' + reg6.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_INC) + ' ' + reg5.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JUMP) + " -12\n");


    // finish add 0
    copy_from_to(reg2, reg3);


    // main loop
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg5.label + " ( main loop ) " + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JZERO) + " 43\n");
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg5.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_INC) + ' ' + reg3.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg3.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SUB) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JNEG) + " 2\n");
    add_line(Asm::get_instruction(Asm::ASM_JUMP) + " 2\n");
    add_line(Asm::get_instruction(Asm::ASM_RESET) + " a\n");

    add_line(Asm::get_instruction(Asm::ASM_JZERO) + " 2\n");
    add_line(Asm::get_instruction(Asm::ASM_JUMP) + " 2\n");
    add_line(Asm::get_instruction(Asm::ASM_JUMP) + " 18\n");

    add_line(Asm::get_instruction(Asm::ASM_DEC) + " a\n");
    add_line(Asm::get_instruction(Asm::ASM_DEC) + ' ' + reg5.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg3.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_RESET) + ' ' + reg6.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_INC) + ' ' + reg6.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SHIFT) + ' ' + reg6.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_INC) + " a\n");
    add_line(Asm::get_instruction(Asm::ASM_DEC) + ' ' + reg6.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_DEC) + ' ' + reg6.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SHIFT) + ' ' + reg6.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg4.label + '\n');

    copy_from_to(reg3, reg2);
    add_line(Asm::get_instruction(Asm::ASM_JUMP) + " -28\n");

    // b_gt
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg3.label + '\n');
    copy_from_to(reg2, reg3);
    add_line(Asm::get_instruction(Asm::ASM_RESET) + ' ' + reg6.label + " ( b gt ) " + '\n');
    add_line(Asm::get_instruction(Asm::ASM_INC) + ' ' + reg6.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SHIFT) + ' ' + reg6.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_DEC) + ' ' + reg5.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_DEC) + ' ' + reg6.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_DEC) + ' ' + reg6.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SHIFT) + ' ' + reg6.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JUMP) + " -15\n");

    // end

    auto block = blockStack.top()->get_code();
    blockStack.pop();
    add_line(Asm::get_instruction(Asm::ASM_RESET) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_RESET) + " a\n");
    add_line(Asm::get_instruction(Asm::ASM_STORE) + ' ' + reg4.label + '\n');

    // check a < 0
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + " (div very begin) " +'\n');
    add_line(Asm::get_instruction(Asm::ASM_JPOS) + " 29\n");
    add_line(Asm::get_instruction(Asm::ASM_JZERO) + " 4\n");
    // a < 0
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JPOS) + " 12\n");
    add_line(Asm::get_instruction(Asm::ASM_JZERO) + " 27\n");
    // both < 0
    add_line(Asm::get_instruction(Asm::ASM_RESET) + ' ' + reg3.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg3.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SUB) + ' ' + reg3.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_RESET) + ' ' + reg3.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg3.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SUB) + ' ' + reg3.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JUMP) + " 13\n");
    // a < 0, b > 0
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_RESET) + ' ' + reg3.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg3.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SUB) + ' ' + reg3.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_ADD) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_DEC) + " a\n");
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_RESET) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_RESET) + " a\n");
    add_line(Asm::get_instruction(Asm::ASM_INC) + " a\n");
    add_line(Asm::get_instruction(Asm::ASM_STORE) + ' ' + reg4.label + '\n');

    add_line(Asm::get_instruction(Asm::ASM_JUMP) + " 19\n");

    // a > 0
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JPOS) + " 15\n");
    add_line(Asm::get_instruction(Asm::ASM_JZERO) + ' ' + std::to_string((block.size() + 16)) + '\n');
    // b < 0
    add_line(Asm::get_instruction(Asm::ASM_RESET) + ' ' + reg3.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg3.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SUB) + ' ' + reg3.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_ADD) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_DEC) + " a\n");
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_RESET) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_RESET) + " a\n");
    add_line(Asm::get_instruction(Asm::ASM_INC) + " a\n");
    add_line(Asm::get_instruction(Asm::ASM_STORE) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JUMP) + " 2\n");


    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');



    std::for_each(block.begin(), block.end(), [&](auto& line){
        add_line(line);
    });
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg5.label + " ( main loop end ) " + '\n');
    //add_line(Asm::get_instruction(Asm::ASM_PUT) + " \n");
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_RESET) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_LOAD) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JPOS) + "3\n");
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JUMP) + " 4\n");
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_RESET) + " a\n");
    add_line(Asm::get_instruction(Asm::ASM_SUB) + ' ' + reg1.label + '\n');
    reg5.free = true;
    reg1.free = true;
    reg2.free = true;
    reg3.free = true;
    reg4.free = true;
}
/*
void CodeGenerator::div(const Value &a, const Value& b) {
    Register& reg1 = MemoryManager::get_free_reg(this);
    reg1.free = false;
    Register& reg2 = MemoryManager::get_free_reg(this);
    reg2.free = false;
    Register& reg3 = MemoryManager::get_free_reg(this);
    reg3.free = false;
    Register& reg4 = MemoryManager::get_free_reg(this);
    reg4.free = false;
    move_number_to_reg(64, reg3);

    // value of b should be in reg1
    if (b.get_type() == Value::TYPE_NUMBER) {
        move_number_to_reg(Util::to_rvalue(b).get_value(), reg1);
    } else {
        move_address_to_reg(Util::to_lvalue(b), reg1);
        add_line(Asm::get_instruction(Asm::ASM_LOAD) + ' ' + reg1.label + '\n');
        add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');
    }

    // value of a should be in reg2
    if (a.get_type() == Value::TYPE_NUMBER) {
        move_number_to_reg(Util::to_rvalue(a).get_value(), reg2);
    } else {
        move_number_to_reg(Util::to_lvalue(a).get_address(), reg2);
        add_line(Asm::get_instruction(Asm::ASM_LOAD) + ' ' + reg2.label + '\n');
        add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + '\n');
    }

    Register& reg5 = MemoryManager::get_free_reg(this);
    reg5.free = false;

    blockStack.emplace(std::make_shared<CodeBlock>());
    // move rem to a, counter to reg3
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg3.label + '\n');
    // copy quot to reg5
    copy_from_to(reg2, reg5);
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg5.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JNEG) + " 2\n");
    add_line(Asm::get_instruction(Asm::ASM_JUMP) + " 4\n");
    add_line(Asm::get_instruction(Asm::ASM_RESET) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SUB) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg5.label + '\n');

    add_line(Asm::get_instruction(Asm::ASM_RESET) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_INC) + ' ' + reg4.label + '\n');
    // rem *= 2
    add_line(Asm::get_instruction(Asm::ASM_SHIFT) + ' ' + reg4.label + '\n');

    // reg2 <- rem, regA <- quot
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + '\n');
    // quot *= 2
    add_line(Asm::get_instruction(Asm::ASM_SHIFT) + ' ' + reg4.label + '\n');

    // reg5 <- quot, regA <- old
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg5.label + '\n');
    copy_from_to(reg5, reg4);
    Register& reg6 = MemoryManager::get_free_reg(this);
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg6.label + '\n');

    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JNEG) + " 2\n");
    add_line(Asm::get_instruction(Asm::ASM_JUMP) + " 5\n");
    add_line(Asm::get_instruction(Asm::ASM_RESET) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg4.label + '\n');
    // regA <- |quot|
    add_line(Asm::get_instruction(Asm::ASM_SUB) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JNEG) + " 7\n");


    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg6.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JNEG) + " 4\n");

    add_line(Asm::get_instruction(Asm::ASM_SUB) + ' ' + reg6.label + '\n');

    add_line(Asm::get_instruction(Asm::ASM_JPOS) + " 2\n");
    add_line(Asm::get_instruction(Asm::ASM_JUMP) + " 2\n");
    // rem++
    add_line(Asm::get_instruction(Asm::ASM_INC) + ' ' + reg2.label + '\n');
    // regA <- rem, reg2 <- trash
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + '\n');


    // regA = rem - divisor
    add_line(Asm::get_instruction(Asm::ASM_SUB) + ' ' + reg1.label + '\n');

    // jump if rem < divisor
    add_line(Asm::get_instruction(Asm::ASM_JNEG) + " 3\n");
    add_line(Asm::get_instruction(Asm::ASM_INC) + ' ' + reg5.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JUMP) + " 2\n");
    add_line(Asm::get_instruction(Asm::ASM_ADD) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg5.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg5.label + '\n');

    add_line(Asm::get_instruction(Asm::ASM_DEC) + ' ' + reg3.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg3.label + '\n');


    auto block = blockStack.top()->get_code();
    blockStack.pop();
    add_line(Asm::get_instruction(Asm::ASM_RESET) + " a\n");

    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JZERO) + ' ' + std::to_string(block.size() + 8) + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + '\n');

    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JZERO) + ' ' + std::to_string(block.size() + 5) + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');

    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg3.label + "(division)" + '\n');

    std::for_each(block.begin(), block.end(), [&](auto& line){
        add_line(line);
    });
    reg5.free = true;


    add_line(Asm::get_instruction(Asm::ASM_JPOS)  + ' ' + std::to_string(-int(block.size())) + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + '\n');
    reg1.free = true;
    reg2.free = true;
    reg3.free = true;
    reg4.free = true;
}*/


void CodeGenerator::mod(const Value &a, const Value& b) {
    Register& reg1 = MemoryManager::get_free_reg(this);
    reg1.free = false;
    Register& reg2 = MemoryManager::get_free_reg(this);
    reg2.free = false;
    Register& reg3 = MemoryManager::get_free_reg(this);
    reg3.free = false;
    Register& reg4 = MemoryManager::get_free_reg(this);
    reg4.free = false;

    // value of b should be in reg1
    move_value_to_reg(b, reg1);

    // value of a should be in reg2
    move_value_to_reg(a, reg2);

    Register& reg5 = MemoryManager::get_free_reg(this);
    reg5.free = false;
    Register& reg6  = MemoryManager::get_free_reg(this);
    reg6.free = false;

    blockStack.emplace(std::make_shared<CodeBlock>());

    add_line(Asm::get_instruction(Asm::ASM_RESET) + " a  (div) \n");
    copy_from_to(reg1, reg4);
    copy_from_to(reg2, reg3);

    add_line(Asm::get_instruction(Asm::ASM_RESET) + ' ' + reg5.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_INC) + ' ' + reg5.label + '\n');

    // count 0 - a + 2 registers
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg4.label + " ( count 0 ) " +'\n');
    add_line(Asm::get_instruction(Asm::ASM_JZERO) + " 11\n");
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_RESET) + " a\n");
    add_line(Asm::get_instruction(Asm::ASM_DEC) + " a\n");

    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SHIFT) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg3.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SHIFT) + ' ' + reg3.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg3.label + '\n');

    add_line(Asm::get_instruction(Asm::ASM_JUMP) + " -11\n");

    // reload b
    copy_from_to(reg1, reg4);

    // add 0
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg3.label + " ( add 0 ) " + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JZERO) + " 12\n");
    add_line(Asm::get_instruction(Asm::ASM_RESET) + ' ' + reg3.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_DEC) + ' ' + reg3.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SHIFT) + ' ' + reg3.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg3.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_RESET) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_INC) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SHIFT) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_INC) + ' ' + reg5.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JUMP) + " -12\n");

    // finish add 0
    copy_from_to(reg2, reg3);

    // main loop
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg5.label + " ( main loop ) " + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JZERO) + " 43\n");
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg5.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_INC) + ' ' + reg3.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg3.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SUB) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JNEG) + " 2\n");
    add_line(Asm::get_instruction(Asm::ASM_JUMP) + " 2\n");
    add_line(Asm::get_instruction(Asm::ASM_RESET) + " a\n");

    add_line(Asm::get_instruction(Asm::ASM_JZERO) + " 2\n");
    add_line(Asm::get_instruction(Asm::ASM_JUMP) + " 2\n");
    add_line(Asm::get_instruction(Asm::ASM_JUMP) + " 18\n");

    add_line(Asm::get_instruction(Asm::ASM_DEC) + " a\n");
    add_line(Asm::get_instruction(Asm::ASM_DEC) + ' ' + reg5.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg3.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_RESET) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_INC) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SHIFT) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_INC) + " a\n");
    add_line(Asm::get_instruction(Asm::ASM_DEC) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_DEC) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SHIFT) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg4.label + '\n');

    copy_from_to(reg3, reg2);
    add_line(Asm::get_instruction(Asm::ASM_JUMP) + " -28\n");

    // b_gt
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg3.label + '\n');
    copy_from_to(reg2, reg3);
    add_line(Asm::get_instruction(Asm::ASM_RESET) + ' ' + reg1.label + " ( b gt ) " + '\n');
    add_line(Asm::get_instruction(Asm::ASM_INC) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SHIFT) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_DEC) + ' ' + reg5.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_DEC) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_DEC) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SHIFT) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JUMP) + " -15\n");

    // end
    auto block = blockStack.top()->get_code();
    blockStack.pop();
    add_line(Asm::get_instruction(Asm::ASM_RESET) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_RESET) + " a\n");
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg6.label + '\n');

    // check a < 0
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + " (div very begin) " +'\n');
    add_line(Asm::get_instruction(Asm::ASM_JPOS) + " 32\n");
    add_line(Asm::get_instruction(Asm::ASM_JZERO) + " 4\n");
    // a < 0
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JPOS) + " 17\n");
    add_line(Asm::get_instruction(Asm::ASM_JZERO) + " 30\n");
    // both < 0
    add_line(Asm::get_instruction(Asm::ASM_RESET) + ' ' + reg3.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg3.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SUB) + ' ' + reg3.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_RESET) + ' ' + reg3.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg3.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SUB) + ' ' + reg3.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_RESET) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_RESET) + " a\n");
    add_line(Asm::get_instruction(Asm::ASM_DEC) + " a\n");
    add_line(Asm::get_instruction(Asm::ASM_DEC) + " a\n");
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg6.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JUMP) + " 11\n");
    // a < 0, b > 0
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_RESET) + ' ' + reg3.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg3.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SUB) + ' ' + reg3.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_RESET) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_RESET) + " a\n");
    add_line(Asm::get_instruction(Asm::ASM_INC) + " a\n");
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg6.label + '\n');

    add_line(Asm::get_instruction(Asm::ASM_JUMP) + " 17\n");

    // a > 0
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JPOS) + " 13\n");
    add_line(Asm::get_instruction(Asm::ASM_JZERO) + ' ' + std::to_string((block.size() + 21)) + '\n');
    // b < 0
    add_line(Asm::get_instruction(Asm::ASM_RESET) + ' ' + reg3.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg3.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SUB) + ' ' + reg3.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_RESET) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_RESET) + " a\n");
    add_line(Asm::get_instruction(Asm::ASM_DEC) + " a\n");
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg6.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JUMP) + " 2\n");


    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');


    std::for_each(block.begin(), block.end(), [&](auto& line){
        add_line(line);
    });
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg5.label + " ( main loop end ) " + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg3.label + " ( main loop end ) " + '\n');
    //add_line(Asm::get_instruction(Asm::ASM_PUT) + " \n");
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_RESET) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg6.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JPOS) + "14\n");
    add_line(Asm::get_instruction(Asm::ASM_JNEG) + "3\n");

    // both positive
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JUMP) + " 15\n");

    // b < 0
    add_line(Asm::get_instruction(Asm::ASM_INC) + " a\n");
    add_line(Asm::get_instruction(Asm::ASM_JNEG) + "6\n");

    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_RESET) + " a\n");
    // TODO - reg 1 is wrong now!!!
    add_line(Asm::get_instruction(Asm::ASM_SUB) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_ADD) + ' ' + reg2.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JUMP) + "8\n");

    add_line(Asm::get_instruction(Asm::ASM_RESET) + " a\n");
    add_line(Asm::get_instruction(Asm::ASM_SUB) + ' ' + reg2.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JUMP) + "5\n");

    // a < 0
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_RESET) + " a\n");
    add_line(Asm::get_instruction(Asm::ASM_ADD) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SUB) + ' ' + reg2.label + '\n');
    reg5.free = true;
    reg6.free = true;
    reg1.free = true;
    reg2.free = true;
    reg3.free = true;
    reg4.free = true;
}
/*
void CodeGenerator::mod(const Value &a, const Value& b) {
    Register& reg1 = MemoryManager::get_free_reg(this);
    reg1.free = false;
    Register& reg2 = MemoryManager::get_free_reg(this);
    reg2.free = false;
    Register& reg3 = MemoryManager::get_free_reg(this);
    reg3.free = false;
    Register& reg4 = MemoryManager::get_free_reg(this);
    reg4.free = false;

    // value of b should be in reg1
    move_value_to_reg(b, reg1);

    // value of a should be in reg2
    move_value_to_reg(a, reg2);

    Register& reg5 = MemoryManager::get_free_reg(this);
    reg5.free = false;
    copy_from_to(reg1, reg3);
    add_line(Asm::get_instruction(Asm::ASM_RESET) + " a\n");
    add_line(Asm::get_instruction(Asm::ASM_RESET) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_DEC) + ' ' + reg4.label + '\n');

    blockStack.emplace(std::make_shared<CodeBlock>());

    add_line(Asm::get_instruction(Asm::ASM_RESET) + " a\n");
    add_line(Asm::get_instruction(Asm::ASM_INC) + " a\n");

    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg3.label + " ( count 0 ) " +'\n');
    add_line(Asm::get_instruction(Asm::ASM_SUB) + ' ' + reg2.label + " ( count 0 ) " +'\n');
    add_line(Asm::get_instruction(Asm::ASM_JPOS) + " 16\n");
    copy_from_to(reg1, reg3);

    add_line(Asm::get_instruction(Asm::ASM_RESET) + " a\n");
    add_line(Asm::get_instruction(Asm::ASM_INC) + " a\n");
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg3.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SHIFT) + ' ' + reg3.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg3.label + '\n');
    copy_from_to(reg3, reg1);
    add_line(Asm::get_instruction(Asm::ASM_INC) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JUMP) + " -17\n");

    add_line(Asm::get_instruction(Asm::ASM_ADD) + ' ' + reg2.label + " ( count 0 ) " +'\n');
    add_line(Asm::get_instruction(Asm::ASM_RESET) + ' ' + reg3.label + " ( count 0 ) " +'\n');
    add_line(Asm::get_instruction(Asm::ASM_DEC) + ' ' + reg3.label + " ( count 0 ) " +'\n');
    add_line(Asm::get_instruction(Asm::ASM_SHIFT) + ' ' + reg3.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');

    // start loop
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JNEG) + " 15\n");
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SUB) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JNEG) + " 2\n");

    add_line(Asm::get_instruction(Asm::ASM_JUMP) + " 2\n");
    add_line(Asm::get_instruction(Asm::ASM_ADD) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_RESET) + " a\n");
    add_line(Asm::get_instruction(Asm::ASM_DEC) + " a\n");
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SHIFT) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_DEC) + ' ' + reg4.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JUMP) + " -15\n");

    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + '\n');


    auto block = blockStack.top()->get_code();
    blockStack.pop();
    /*add_line(Asm::get_instruction(Asm::ASM_RESET) + " a\n");

    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JZERO) + ' ' + std::to_string(block.size() + 8) + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + '\n');

    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JZERO) + ' ' + std::to_string(block.size() + 5) + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');

    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg3.label + "(division)" + '\n');*/
/*
    std::for_each(block.begin(), block.end(), [&](auto& line){
        add_line(line);
    });
    reg5.free = true;


    reg1.free = true;
    reg2.free = true;
    reg3.free = true;
    reg4.free = true;
}*/

void CodeGenerator::write(const Value &a) {
    Register& reg = MemoryManager::get_free_reg(this);
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
    Register& reg = MemoryManager::get_free_reg(this);
    reg.free = false;

    Register& h = MemoryManager::get_h();
    if (h.value != nullptr) {
        std::cout << "h: " << Util::to_lvalue(*h.value).getId() << '\n';
        if (a.get_type() == Value::TYPE_VAR) {
            std::cout << "a: " << Util::to_lvalue(a).getId() << '\n';
        }
    } else {
        std::cout << "h is null\n";
    }

    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg.label + " (assign) " + '\n');
    if (!loopStack.empty() || !branchStack.empty()) {
        if (h.value == nullptr || !(*h.value == a)) {
            move_address_to_reg(Util::to_lvalue(a), MemoryManager::get_a());
            add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg.label + '\n');
            add_line(Asm::get_instruction(Asm::ASM_STORE) + ' ' + reg.label + '\n');
        } else {
            std::cout << "ohh... wait, we are twins!\n";
            add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg.label + " (assign) " + '\n');
            add_line(Asm::get_instruction(Asm::ASM_SWAP) + " h\n");
        }
        reg.free = true;
        return;
    }
    if (h.value == nullptr) {
        if (a.get_type() != Value::TYPE_ARRAY) {
            h.value = std::make_shared<LValue const>(Util::to_lvalue(a));
            add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg.label + " (assign) " + '\n');
            add_line(Asm::get_instruction(Asm::ASM_SWAP) + " h\n");

        } else {
            move_address_to_reg(Util::to_lvalue(a), MemoryManager::get_a());
            add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg.label + '\n');
            add_line(Asm::get_instruction(Asm::ASM_STORE) + ' ' + reg.label + '\n');
        }
    } else if (h.value->get_priority() <= a.get_priority() && a.get_type() != Value::TYPE_ARRAY) {
        move_address_to_reg(Util::to_lvalue(*h.value), MemoryManager::get_a());
        add_line(Asm::get_instruction(Asm::ASM_SWAP) + " h\n");
        add_line(Asm::get_instruction(Asm::ASM_STORE) + " h\n");
        add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg.label + '\n');
        add_line(Asm::get_instruction(Asm::ASM_SWAP) + " h\n");

        h.value = std::make_shared<LValue const>(Util::to_lvalue(a));
    } else {
        move_address_to_reg(Util::to_lvalue(a), MemoryManager::get_a());
        add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg.label + '\n');
        add_line(Asm::get_instruction(Asm::ASM_STORE) + ' ' + reg.label + '\n');
    }

    /*move_address_to_reg(Util::to_lvalue(a), MemoryManager::get_a());
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_STORE) + ' ' + reg.label + '\n');*/
    reg.free = true;
}

std::vector<std::string> CodeGenerator::generate_asm_code() {
    return lines;
}

void CodeGenerator::eq(const Value &a, const Value &b) {
    Register& reg1 = MemoryManager::get_free_reg(this);
    reg1.free = false;
    Register& reg2 = MemoryManager::get_free_reg(this);
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
    Register& reg1 = MemoryManager::get_free_reg(this);
    reg1.free = false;
    Register& reg2 = MemoryManager::get_free_reg(this);
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
    Register& reg1 = MemoryManager::get_free_reg(this);
    reg1.free = false;
    Register& reg2 = MemoryManager::get_free_reg(this);
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
    Register& reg1 = MemoryManager::get_free_reg(this);
    reg1.free = false;
    Register& reg2 = MemoryManager::get_free_reg(this);
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
    Register& reg1 = MemoryManager::get_free_reg(this);
    reg1.free = false;
    Register& reg2 = MemoryManager::get_free_reg(this);
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
    Register& reg1 = MemoryManager::get_free_reg(this);
    reg1.free = false;
    Register& reg2 = MemoryManager::get_free_reg(this);
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
    Register& helper = MemoryManager::get_free_reg(this);
    add_line(Asm::get_instruction(Asm::ASM_RESET) + " a\n");
    add_line(Asm::get_instruction(Asm::ASM_RESET) + ' ' + helper.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_INC) + ' ' + helper.label + '\n');
    int i = 63;

    long long numCopy = number;

    if (number < 0) {
        numCopy *=  -1;
    }
    std::bitset<64> bits(numCopy);


    while (bits[i] == 0 && i >= 0) {
        i--;
    }
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
        Register& helper = MemoryManager::get_free_reg(this);
        helper.free = false;
        Register& helper2 = MemoryManager::get_free_reg(this);

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
    Register& reg1 = MemoryManager::get_free_reg(this);
    reg1.free = false;
    Register& reg2 = MemoryManager::get_free_reg(this);
    reg2.free = false;


    move_value_to_reg(from, reg1);
    move_value_to_reg(to, reg2);

    Register& reg3 = MemoryManager::get_free_reg(this);
    reg3.free = false;
    move_address_to_reg(*(loop->iterator), reg3);
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_STORE) + ' ' + reg3.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');
    move_address_to_reg(*(loop->counter), reg3);
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_STORE) + ' ' + reg3.label + '\n');
    reg3.free = true;
    blockStack.emplace(std::make_shared<CodeBlock>());
    loopStack.push(loop);
    reg1.free = true;
    reg2.free = true;
}

std::shared_ptr<Loop> CodeGenerator::end_for() {
    auto ptr = loopStack.top();
    loopStack.pop();

    inc(*(ptr->iterator));

    Register& reg1 = MemoryManager::get_free_reg(this);
    reg1.free = false;
    Register& reg2 = MemoryManager::get_free_reg(this);
    reg2.free = false;
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');
    move_value_to_reg(*(ptr->counter), MemoryManager::get_a());
    add_line(Asm::get_instruction(Asm::ASM_SUB) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JNEG) + " 2\n");
    reg1.free = true;
    reg2.free = true;

    auto block = blockStack.top()->get_code();

    blockStack.pop();

    add_line(Asm::get_instruction(Asm::ASM_SUB) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JNEG) + ' ' + std::to_string(block.size() + 2) + '\n');

    std::for_each(block.begin(), block.end(), [&](auto& line) {
        add_line(line);
    });
    add_line(Asm::get_instruction(Asm::ASM_JUMP) + ' ' + std::to_string(-(int)block.size()) + '\n');

    return ptr;
}

std::shared_ptr<Loop> CodeGenerator::end_down_to() {
    auto ptr = loopStack.top();
    loopStack.pop();

    dec(*(ptr->iterator));

    Register& reg1 = MemoryManager::get_free_reg(this);
    reg1.free = false;
    Register& reg2 = MemoryManager::get_free_reg(this);
    reg2.free = false;
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');
    move_address_to_reg(*(ptr->counter), reg2);
    add_line(Asm::get_instruction(Asm::ASM_LOAD) + ' ' + reg2.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SUB) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JPOS) + " 2\n");
    reg1.free = true;
    reg2.free = true;

    auto block = blockStack.top()->get_code();

    blockStack.pop();

    add_line(Asm::get_instruction(Asm::ASM_SUB) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_JPOS) + ' ' + std::to_string(block.size() + 2) + '\n');

    std::for_each(block.begin(), block.end(), [&](auto& line) {
        add_line(line);
    });
    add_line(Asm::get_instruction(Asm::ASM_JUMP) + ' ' + std::to_string(-(int)block.size()) + '\n');

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
    Register& reg = MemoryManager::get_free_reg(this);
    reg.free = false;
    move_address_to_reg(value, reg);
    add_line(Asm::get_instruction(Asm::ASM_LOAD) + ' ' + reg.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_INC) + " a\n");
    add_line(Asm::get_instruction(Asm::ASM_STORE) + ' ' + reg.label + '\n');
    reg.free = true;
}

void CodeGenerator::dec(const LValue &value) {
    Register& reg = MemoryManager::get_free_reg(this);
    reg.free = false;
    move_address_to_reg(value, reg);
    add_line(Asm::get_instruction(Asm::ASM_LOAD) + ' ' + reg.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_DEC) + " a\n");
    add_line(Asm::get_instruction(Asm::ASM_STORE) + ' ' + reg.label + '\n');
    reg.free = true;
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
    /*if (value.get_type() == Value::TYPE_NUMBER) {
        move_number_to_reg(Util::to_rvalue(value).get_value(), MemoryManager::get_a());
    } else {
        Register& reg = MemoryManager::get_free_reg(this);
        reg.free = false;
        move_address_to_reg(Util::to_lvalue(value), reg);
        add_line(Asm::get_instruction(Asm::ASM_LOAD) + ' ' + reg.label + '\n');
        reg.free = true;
    }*/
}

void CodeGenerator::copy_from_to(const Register &reg1, const Register &reg2) {
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_RESET) + " a\n");
    add_line(Asm::get_instruction(Asm::ASM_ADD) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + '\n');
}

void CodeGenerator::mod(const Value &a, const Value &b, const Value& helper) {

/*    div(a, b);
    Register& reg = MemoryManager::get_free_reg(this);
    reg.free = false;
    Register& reg1 = MemoryManager::get_free_reg(this);
    reg1.free = false;
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');
    move_address_to_reg(Util::to_lvalue(helper), reg);
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_STORE) + ' ' + reg.label + '\n');
    reg.free = true;
    reg1.free = true;
    mul(helper, b);

    reg.free = false;
    reg1.free = false;

    Register& reg2 = MemoryManager::get_free_reg(this);
    reg2.free = false;

    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg1.label + '\n');

    if (a.get_type() == Value::TYPE_NUMBER) {
        move_number_to_reg(Util::to_rvalue(a).get_value(), reg2);
    } else {
        move_number_to_reg(Util::to_lvalue(a).get_address(), reg2);
        add_line(Asm::get_instruction(Asm::ASM_LOAD) + ' ' + reg2.label + '\n');
        add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + '\n');
    }
    add_line(Asm::get_instruction(Asm::ASM_SWAP) + ' ' + reg2.label + '\n');
    add_line(Asm::get_instruction(Asm::ASM_SUB) + ' ' + reg1.label + '\n');
    reg.free = true;
    reg1.free = true;
    reg2.free = true;
*/
}
