#include "Variable.h"

Variable::Variable(std::string name, uint address, uint start, uint end) {
    this->name = name;
    this->address = address;
    this->start = start;
    this->end = end;
}

uint Variable::getAddress() {
    return this->address;
}
