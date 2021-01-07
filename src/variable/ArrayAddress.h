#ifndef ARRAYADDRESS_H
#define ARRAYADDRESS_H


#include "Variable.h"

class ArrayAddress : public Variable {
public:
    ArrayAddress(std::string name, uint address, Variable* index);
    Variable* getIndex();

private:
    Variable* index;
};

#endif //ARRAYADDRESS_H
