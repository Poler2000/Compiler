#include "memory_manager.h"

uint64_t MemoryManager::memoryPtr = 0;

uint64_t MemoryManager::allocate(uint64_t size) {
    uint64_t currentPtr = memoryPtr;
    memoryPtr += size;

    return currentPtr;
}
