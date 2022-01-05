#ifndef COMPILER_V_0_1_LABEL_GENERATOR_H
#define COMPILER_V_0_1_LABEL_GENERATOR_H


#include <string>

class LabelGenerator {
public:
    static std::string get_new_label();

private:
    static uint64_t labelCounter;
};


#endif //COMPILER_V_0_1_LABEL_GENERATOR_H
