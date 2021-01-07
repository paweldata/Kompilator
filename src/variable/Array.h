#ifndef ARRAY_H
#define ARRAY_H


#include <map>

#include "Variable.h"
#include "ArrayAddress.h"

class Array : public Variable {
public:
    Array(std::string name, uint address, uint start, uint end);
    Variable* getOneAddress(uint index);
    Variable* getOneAddress(std::string index);

private:
    std::map<uint, Variable*> memory;
    uint start;
    uint end;
};

#endif //ARRAY_H
