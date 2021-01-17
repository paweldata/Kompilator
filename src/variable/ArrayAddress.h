#ifndef ARRAYADDRESS_H
#define ARRAYADDRESS_H


#include "Variable.h"

class ArrayAddress : public Variable {
public:
    ArrayAddress(std::string name, uint arrAddress, uint elemAddress, Variable* index);
    Variable* getIndex();
    uint getArrAddress();
    uint getElemAddress();

private:
    void functionForDynamicCast() {};

    uint arrAddress;
    uint elemAddress;
    Variable* index;
};

#endif //ARRAYADDRESS_H
