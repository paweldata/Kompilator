#include <assert.h>
#include <algorithm>

#include "Memory.h"
#include "variable/Array.h"
#include "variable/SimpleVariable.h"

Memory::Memory() {
    this->variables = std::map<std::string, Variable*>();
    this->registers = std::vector<Register>(REGISTERSNUMBER, Register{-1, false});
}

Variable* Memory::getVariable(std::string name) {
    this->checkIfVariableNotExitsts(name);
    this->checkIfTryUseArrayAsVariable(name);
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

    if (Array* arr = dynamic_cast<Array*>(var)) {
        this->checkIfVariableNotExitsts(index);
        Variable* var = this->variables[index];
        return arr->getOneAddress(var);
    }
    throw (std::string) var->getName() + " is not an array";
}

Variable* Memory::getConstant(uint64_t value) {
    return new Constant(value, 0);
}

void Memory::setVariable(std::string name) {
    this->checkIfVariableAlreadyExists(name);
    this->variables[name] = new SimpleVariable(name, this->freeMemPtr);
    this->freeMemPtr++;
}

void Memory::setArray(std::string name, uint start, uint end) {
    this->checkIfVariableAlreadyExists(name);
    this->variables[name] = new Array(name, this->freeMemPtr, start, end);
    this->freeMemPtr += end - start + 1;
}

Iterator* Memory::setIterator(std::string name) {
    this->checkIfVariableAlreadyExists(name);
    this->variables[name] = new Iterator(name, this->freeMemPtr);
    this->freeMemPtr++;
    return dynamic_cast<Iterator*>(this->variables[name]);
}

void Memory::deleteIterator(Iterator* it) {
    this->variables.erase(it->getName());
    delete it;
}

std::string Memory::getFreeRegister() {
    for (int i = 0; i < REGISTERSNUMBER; i++) {
        uint regNumber = (i + this->lastSentRegNumber + 1) % REGISTERSNUMBER;
        if (not this->registers[regNumber].isUsed) {
            this->registers[regNumber].isUsed = true;
            this->lastSentRegNumber = regNumber;
            return std::string(1, (char)('a' + regNumber));
        }
    }
    throw (std::string) "Error: there is no free register";
}

std::pair<std::string, bool> Memory::getFreeRegister(uint64_t value) {
    for (int i = 0; i < REGISTERSNUMBER; i++) {
        if (not this->registers[i].isUsed and this->registers[i].value == value) {
            this->registers[i].isUsed = true;
            this->lastSentRegNumber = i;
            return std::make_pair(std::string(1, (char)('a' + i)), true);
        }
    }

    return std::make_pair(this->getFreeRegister(), false);
}

std::pair<std::string, bool> Memory::getFreeRegister(Variable* var) {
    if (auto arrAddr = dynamic_cast<ArrayAddress*>(var)) {
        return std::make_pair(this->getFreeRegister(), false);
    }
    return this->getFreeRegister(var->getAddress());
}

void Memory::freeRegister(std::string reg, int value) {
    uint regNumber = (uint)(reg[0] - 'a');
    assert(regNumber < REGISTERSNUMBER);
    assert(this->registers[regNumber].isUsed);

    this->registers[regNumber].value = value;
    this->registers[regNumber].isUsed = false;
}

void Memory::resetRegistersValue() {
    for (Register& reg : this->registers)
        reg.value = -1;
}

void Memory::assertFreeRegisters() {
    for (Register reg : this->registers)
        assert(not reg.isUsed);
}

void Memory::checkIfVariableNotExitsts(std::string name) {
    if (this->variables.find(name) == this->variables.end())
        throw (std::string) name + " not defined";
}

void Memory::checkIfVariableAlreadyExists(std::string name) {
    if (this->variables.find(name) != this->variables.end())
        throw (std::string) name + " already defined";
}

void Memory::checkIfTryUseArrayAsVariable(std::string name) {
    if (auto arr = dynamic_cast<Array*>(this->variables[name]))
        throw (std::string) name + " is not an array";
}
