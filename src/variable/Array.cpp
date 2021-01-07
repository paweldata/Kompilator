#include "Array.h"

Array::Array(std::string name, uint address, uint start, uint end) 
    : Variable(name, address) {
    this->memory = std::map<uint, Variable*>();
    this->start = start;
    this->end = end;
}

Variable* Array::getOneAddress(uint index) {
    if (index < this->start || index > this->end)
        throw (std::string) "wrong index of an array " + this->name + ", index : " + std::to_string(index);
    
    if (this->memory.find(index) != this->memory.end())
        return this->memory[index];
    
    std::string newName = this->name + "(" + std::to_string(index) + ")";
    uint newAddress = this->address - this->start + index;
    Variable* var = new Variable(newName, newAddress);
    this->memory[index] = var;
    return var;
}

Variable* Array::getOneAddress(std::string index) {
    
}
