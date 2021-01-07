#include "Variable.h"

Variable::Variable(std::string name, uint address, uint start, uint end) {
    this->name = name;
    this->address = address;
    this->start = start;
    this->end = end;
    this->initialized = false;

    this->isArray = false;
    this->isArrayWithIndex = false;
    this->index = nullptr;
}

uint Variable::getAddress() {
    if (not initialized)
        throw (std::string) this->name + " not initialized";
    return this->address;
}

void Variable::initialize() {
    this->initialized = true;
}

void Variable::setToArray() {
    this->isArray = true;
}

uint Variable::getAddress(uint arrayIndex) {
    if (not this->isArray)
        throw (std::string) this->name + " is not an array";
    if (arrayIndex < this->start || arrayIndex > this->end)
        throw (std::string) "wrong index of an array " + this->name + " , index : ", std::to_string(arrayIndex);
    return this->address - this->start + arrayIndex;
}

void Variable::setToArrayWithIndex(Variable* index) {
    this->isArrayWithIndex = true;
    this->index = index;
}

Variable* Variable::getIndex() {
    return this->index;
}
