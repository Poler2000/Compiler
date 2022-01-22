#include <iostream>
#include <asm.h>
#include <util.h>
#include "memory_manager.h"

uint64_t MemoryManager::memoryPtr = 1;

std::array<Register, MemoryManager::nofRegisters> MemoryManager::registers = {{
        {'a', true},
        {'b', true},
        {'c', true},
        {'d', true},
        {'e', true},
        {'f', true},
        {'g', true},
        {'h', true},
}};

uint64_t MemoryManager::allocate(uint64_t size) {
    uint64_t currentPtr = memoryPtr;
    memoryPtr += size;

    return currentPtr;
}

Register &MemoryManager::get_a() {
    return registers[0];
}
/*
Register &MemoryManager::get_free_reg() {
    for (int i = 1; i < nofRegisters; i++) {
        if (registers[i].free) {
            return registers[i];
        }
    }

    std::cerr << "Error: no free register\n";
    exit(EXIT_FAILURE);
}*/

std::shared_ptr<Register> MemoryManager::check_for_value(Value & value) {
    for (auto& reg: registers) {
        if (*(reg.value) == value) {
            return std::make_shared<Register>(reg);
        }
    }
    return nullptr;
}

Register &MemoryManager::get_free_reg(CodeGenerator *generator) {
    for (int i = 1; i < nofRegisters - 1; i++) {
        if (registers[i].free && registers[i].value == nullptr) {
            return registers[i];
        }
    }
/*
    std::cout << "hello\n";
    for (int i = 1; i < nofRegisters; i++) {
        if (registers[i].free) {
            if (registers[i].value->get_type() == Value::TYPE_VAR) {
                generator->move_address_to_reg(Util::to_lvalue(*registers[i].value), MemoryManager::get_a());
                generator->add_line(std::string(Asm::get_instruction(Asm::ASM_SWAP)) + ' ' + registers[i].label + '\n');
                generator->add_line(std::string(Asm::get_instruction(Asm::ASM_STORE)) + ' ' + registers[i].label + '\n');
            }

            return registers[i];
        }
    }*/

    std::cerr << "Error: no free register\n";
    exit(EXIT_FAILURE);
}

Register &MemoryManager::get_h() {
    return registers[7];
}
