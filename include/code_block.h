//
// Created by pawel on 05.01.2022.
//

#ifndef COMPILER_V_0_1_CODE_BLOCK_H
#define COMPILER_V_0_1_CODE_BLOCK_H


#include <vector>
#include <string>

class CodeBlock {
public:
    void add_line(const std::string& line);
    std::vector<std::string> get_code();
private:
    std::vector<std::string> code;
};


#endif //COMPILER_V_0_1_CODE_BLOCK_H
