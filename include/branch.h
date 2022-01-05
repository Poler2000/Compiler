#ifndef COMPILER_V_0_1_BRANCH_H
#define COMPILER_V_0_1_BRANCH_H

struct Branch {
    uint64_t elseOffset;
    uint64_t endOffset;
    bool hasElse;
    std::vector<std::string> ifCode;
    std::vector<std::string> elseCode;
};

#endif //COMPILER_V_0_1_BRANCH_H
