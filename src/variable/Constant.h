#ifndef CONSTANT_H
#define CONSTANT_H


#include "Variable.h"
#include "ArrayAddress.h"

class Constant : public Variable {
public:
    Constant(uint64_t value, uint address) : Variable(std::to_string(value), address) {
        this->value = value;
        this->initialize();
    }

    uint64_t getValue() { return this->value; }

private:
    void functionForDynamicCast() {};

    uint64_t value;
};

#endif //CONSTANT_H
