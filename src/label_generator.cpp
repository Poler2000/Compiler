#include "label_generator.h"

uint64_t LabelGenerator::labelCounter = 0;

std::string LabelGenerator::get_new_label() {
    return std::string("label" + std::to_string(labelCounter++));
}
