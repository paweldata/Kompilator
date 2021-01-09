#include "Constant.h"

Constant::Constant(uint value, uint address) : Variable(std::to_string(value), address) {
    this->value = value;
    this->initialize();
}

uint Constant::getValue() {
    return this->value;
}
