#include "Variable.h"

Variable::Variable(std::string name, uint address, uint start, uint end) {
    this->name = name;
    this->address = address;
    this->start = start;
    this->end = end;
    this->initialized = false;
}

uint Variable::getAddress() {
    if (not initialized)
        throw (std::string) this->name + " not initialized";
    return this->address;
}

void Variable::initialize() {
    this->initialized = true;
}
