#ifndef ARRAYADDRESS_H
#define ARRAYADDRESS_H


#include "Variable.h"

class ArrayAddress : public Variable {
public:
    ArrayAddress(std::string name, uint arrAddress, uint elemAddress, Variable* index)
        : Variable(name, arrAddress) {
        this->index = index;
        this->arrAddress = arrAddress;
        this->elemAddress = elemAddress;
        this->initialize();
    }

    Variable* getIndex() { return this->index; }
    uint getArrAddress() { return this->arrAddress; }
    uint getElemAddress() { return this->elemAddress; }

private:
    void functionForDynamicCast() {};

    uint arrAddress;
    uint elemAddress;
    Variable* index;
};

#endif //ARRAYADDRESS_H
