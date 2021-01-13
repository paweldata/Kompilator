#ifndef ITERATOR_H
#define ITERATOR_H


#include <string>

#include "Variable.h"

class Iterator : public Variable {
public:
    Iterator(std::string name, uint address) : Variable(name, address) {};

private:
    void functionForDynamicCast() {};
};

#endif //ITERATOR_H
