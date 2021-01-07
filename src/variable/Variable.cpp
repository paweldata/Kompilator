#include "Variable.h"

Variable::Variable(std::string name, uint address) {
    this->name = name;
    this->address = address;
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

std::string Variable::getName() {
    return this->name;
}
