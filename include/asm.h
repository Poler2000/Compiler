#ifndef COMPILER_V_0_1_ASM_H
#define COMPILER_V_0_1_ASM_H


class Asm {
public:
    enum Instruction {
        ASM_GET,
        ASM_PUT,
        ASM_LOAD,
        ASM_STORE,
        ASM_ADD,
        ASM_SUB,
        ASM_SHIFT,
        ASM_SWAP,
        ASM_RESET,
        ASM_INC,
        ASM_DEC,
        ASM_JUMP,
        ASM_JPOS,
        ASM_JZERO,
        ASM_JNEG,
        ASM_HALT,
    };
};

#endif //COMPILER_V_0_1_ASM_H
