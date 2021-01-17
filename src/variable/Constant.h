#ifndef CONSTANT_H
#define CONSTANT_H


#include "Variable.h"
#include "ArrayAddress.h"

class Constant : public Variable {
public:
    Constant(uint64_t value, uint address);
    uint64_t getValue();

private:
    void functionForDynamicCast() {};

    uint64_t value;
};

#endif //CONSTANT_H
