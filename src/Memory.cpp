#include "Memory.h"

Memory::Memory() {
    this->variables = std::map<std::string, Variable>();
    this->freeMemPtr = 0;
}

Variable Memory::getVariable(std::string name) {
    this->checkIfVariableNotExitsts(name);
    return this->variables[name];
}

void Memory::setVariable(std::string name) {
    this->checkIfVariableAlreadyExists(name);
    this->variables[name] = Variable(name, this->freeMemPtr, 0, 0);
    this->freeMemPtr++;
}

void Memory::setArray(std::string name, uint start, uint end) {
    this->checkIfVariableAlreadyExists(name);
    this->checkArraySize(start, end);
    this->variables[name] = Variable(name, this->freeMemPtr, start, end);
    this->freeMemPtr += end - start + 1;
}

void Memory::checkIfVariableNotExitsts(std::string name) {
    if (this->variables.find(name) == this->variables.end())
        throw (std::string) name + " not initialized";
}

void Memory::checkIfVariableAlreadyExists(std::string name) {
    if (this->variables.find(name) != this->variables.end())
        throw (std::string) name + " already initialized";
}

void Memory::checkArraySize(uint start, uint end) {
    if (start > end)
        throw (std::string) "wrong array size";
}
