#include "asm.h"

std::string_view Asm::get_instruction(Asm::Instruction i) {
    return instructions[i];
}
