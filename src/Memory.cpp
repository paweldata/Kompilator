#include <assert.h>
#include <algorithm>

#include "Memory.h"
#include "variable/Array.h"

Memory::Memory() {
    this->variables = std::map<std::string, Variable*>();
    this->registers = std::vector<Register>(REGISTERSNUMBER, Register{-1, false});
    this->freeMemPtr = 0;
}

Variable* Memory::getVariable(std::string name) {
    this->checkIfVariableNotExitsts(name);
    return this->variables[name];
}

Variable* Memory::getArrayVariable(std::string name, uint index) {
    this->checkIfVariableNotExitsts(name);
    Variable* var = this->variables[name];
    if (Array* arr = static_cast<Array*>(var))
        return arr->getOneAddress(index);
    throw (std::string) var->getName() + " is not an array";
}

Variable* Memory::getArrayVariable(std::string name, std::string index) {
    this->checkIfVariableNotExitsts(name);
    Variable* var = this->variables[name];

    if (Array* arr = static_cast<Array*>(var)) {
        this->checkIfVariableNotExitsts(index);
        Variable* var = this->variables[index];
        return arr->getOneAddress(var);
    }
    throw (std::string) var->getName() + " is not an array";
}

Variable* Memory::getConstant(uint value) {
    for (Constant* constant : this->constants)
        if (constant->getValue() == value)
            return constant;
    
    Constant* constant = new Constant(value, this->freeMemPtr);
    this->freeMemPtr++;
    this->constants.push_back(constant);
    return constant;
}

void Memory::setVariable(std::string name) {
    this->checkIfVariableAlreadyExists(name);
    this->variables[name] = new Variable(name, this->freeMemPtr);
    this->freeMemPtr++;
}

void Memory::setArray(std::string name, uint start, uint end) {
    this->checkIfVariableAlreadyExists(name);
    this->variables[name] = new Array(name, this->freeMemPtr, start, end);
    this->freeMemPtr += end - start + 1;
}

std::string Memory::getFreeRegister() {
    for (int i = 0; i < REGISTERSNUMBER; i++)
        if (not this->registers[i].isUsed) {
            this->registers[i].isUsed = true;
            return std::string(1, (char)('a' + i));
        }
    throw "Error: there is no free register";
}

void Memory::freeRegister(std::string reg) {
    uint regNumber = (uint)(reg[0] - 'a');
    assert(regNumber < REGISTERSNUMBER);
    assert(this->registers[regNumber].isUsed == true);
    this->registers[regNumber].isUsed = false;
}

void Memory::checkIfVariableNotExitsts(std::string name) {
    if (this->variables.find(name) == this->variables.end())
        throw (std::string) name + " not defined";
}

void Memory::checkIfVariableAlreadyExists(std::string name) {
    if (this->variables.find(name) != this->variables.end())
        throw (std::string) name + " already defined";
}
