#include "Constant.h"

Constant::Constant(uint64_t value, uint address) : Variable(std::to_string(value), address) {
    this->value = value;
    this->initialize();
}

uint64_t Constant::getValue() {
    return this->value;
}
