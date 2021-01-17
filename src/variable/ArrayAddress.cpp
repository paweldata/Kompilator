#include "ArrayAddress.h"

ArrayAddress::ArrayAddress(std::string name, uint arrAddress, uint elemAddress, Variable* index)
    : Variable(name, arrAddress) {
    this->index = index;
    this->arrAddress = arrAddress;
    this->elemAddress = elemAddress;
    this->initialize();
}

uint ArrayAddress::getArrAddress() {
    return this->arrAddress;
}

uint ArrayAddress::getElemAddress() {
    return this->elemAddress;
}

Variable* ArrayAddress::getIndex() {
    return this->index;
}
