#include "asm.h"

std::string Asm::instructions[16] = {
    "GET",
    "PUT",
    "LOAD",
    "STORE",
    "ADD",
    "SUB",
    "SHIFT",
    "SWAP",
    "RESET",
    "INC",
    "DEC",
    "JUMP",
    "JPOS",
    "JZERO",
    "JNEG",
    "HALT",
};

std::string& Asm::get_instruction(Asm::Instruction i) {
    return instructions[i];
}
