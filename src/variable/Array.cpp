#include "Array.h"
#include "ArrayAddress.h"

Array::Array(std::string name, uint address, uint start, uint end) 
    : Variable(name, address) {
    this->memory = std::map<uint, Variable*>();
    this->start = start;
    this->end = end;

    this->checkSize();
}

Variable* Array::getOneAddress(uint index) {
    this->checkIndex(index);
    
    if (this->memory.find(index) != this->memory.end())
        return this->memory[index];
    
    std::string newName = this->name + "(" + std::to_string(index) + ")";
    uint newAddress = this->address - this->start + index;
    Variable* var = new Variable(newName, newAddress);
    this->memory[index] = var;
    return var;
}

Variable* Array::getOneAddress(Variable* var) {
    std::string newName = this->name + "(" + var->getName() + ")";
    Variable* newVar = new ArrayAddress(newName, this->address, var);
    newVar->initialize();
    return newVar;
}

void Array::checkSize() {
    if (this->start > this->end)
        throw (std::string) "wrong array size";
}

void Array::checkIndex(uint index) {
    if (index < this->start || index > this->end)
        throw (std::string) "wrong index of an array " + this->name + ", index : " + std::to_string(index);
}
