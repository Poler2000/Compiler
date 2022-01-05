//
// Created by pawel on 05.01.2022.
//

#include "code_block.h"

void CodeBlock::add_line(const std::string &line) {
    code.emplace_back(line);
}

std::vector<std::string> CodeBlock::get_code() {
    return code;
}
