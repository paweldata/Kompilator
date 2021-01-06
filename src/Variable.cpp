#include "Variable.h"

Variable::Variable(std::string name, uint address, uint first) {
    this->name = name;
    this->address = address;
    this->first = first;
}

uint Variable::getAddress() {
    return this->address;
}
