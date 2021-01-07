#include <assert.h>
#include <algorithm>

#include "Memory.h"

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
    Variable* array = this->variables[name];

    std::string newName = name + "(" + std::to_string(index) + ")";
    uint newIndex = array->getAddress(index);
    return new Variable(newName, newIndex, 0, 0);
}

Variable* Memory::getArrayVariable(std::string name, std::string index) {
    this->checkIfVariableNotExitsts(name);
    this->checkIfVariableNotExitsts(index);
    Variable* array = this->variables[name];
    Variable* indexVar = this->variables[index];

    std::string newName = name + "(" + index + ")";
    uint newAddress = array->getAddress();
    Variable* var = new Variable(newName, newAddress, 0, 0);
    var->setToArrayWithIndex(indexVar);
    return var;
}

void Memory::setVariable(std::string name) {
    this->checkIfVariableAlreadyExists(name);
    this->variables[name] = new Variable(name, this->freeMemPtr, 0, 0);
    this->freeMemPtr++;
}

void Memory::setArray(std::string name, uint start, uint end) {
    this->checkIfVariableAlreadyExists(name);
    this->checkArraySize(start, end);
    this->variables[name] = new Variable(name, this->freeMemPtr, start, end);
    this->variables[name]->setToArray();
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

void Memory::checkArraySize(uint start, uint end) {
    if (start > end)
        throw (std::string) "wrong array size";
}
