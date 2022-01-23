#include <iostream>
#include <asm.h>
#include "memory_manager.h"

uint64_t MemoryManager::memoryPtr = 0;

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


Register &MemoryManager::get_free_reg() {
    for (int i = 1; i < nofRegisters - 2; i++) {
        if (registers[i].free && registers[i].value == nullptr) {
            return registers[i];
        }
    }

    std::cerr << "Error: no free register\n";
    exit(EXIT_FAILURE);
}

Register &MemoryManager::get_h() {
    return registers[7];
}

Register &MemoryManager::get_g() {
    return registers[6];
}
