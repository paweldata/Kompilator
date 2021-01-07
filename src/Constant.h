#ifndef CONSTANT_H
#define CONSTANT_H


#include <string>

#include "Variable.h"

class Constant : public Variable {
public:
    Constant(uint value, uint address) : Variable(std::to_string(value), address, 0, 0) {
        this->value = value;
        this->initialize();
    }

    uint getValue() { return this->value; }

private:
    uint value;
};

#endif //CONSTANT_H
