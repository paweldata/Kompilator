#include "ArrayAddress.h"

ArrayAddress::ArrayAddress(std::string name, uint address, Variable* index)
    : Variable(name, address) {
    this->index = index;
    this->initialize();
}

Variable* ArrayAddress::getIndex() {
    return this->index;
}
