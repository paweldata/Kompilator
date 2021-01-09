#ifndef SIMPLEVARIABLE_H
#define SIMPLEVARIABLE_H


#include <string>

#include "Variable.h"

class SimpleVariable : public Variable {
public:
    SimpleVariable(std::string name, uint address) : Variable(name, address) {};

private:
    void functionForDynamicCast() {};
};

#endif //SIMPLEVARIABLE_H