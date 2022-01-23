#ifndef COMPILER_V_0_1_MEMORY_MANAGER_H
#define COMPILER_V_0_1_MEMORY_MANAGER_H


#include <cstdint>
#include <array>

#include "register.h"

class MemoryManager {
public:
    static uint64_t allocate(uint64_t size);
    static Register &get_free_reg();
    static Register &get_a();
    static Register &get_h();
    static Register &get_g();
private:
    static constexpr size_t nofRegisters = 8;
    static uint64_t memoryPtr;
    static std::array<Register, nofRegisters> registers;
};


#endif //COMPILER_V_0_1_MEMORY_MANAGER_H
