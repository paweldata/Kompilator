#ifndef CONSTANT_H
#define CONSTANT_H


#include "Variable.h"
#include "ArrayAddress.h"

class Constant : public Variable {
public:
    Constant(uint value, uint address);
    uint getValue();

private:
    uint value;
};

#endif //CONSTANT_H
