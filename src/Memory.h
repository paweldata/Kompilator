#ifndef MEMORY_H
#define MEMORY_H


#include <map>
#include <vector>

#include "Variable.h"

constexpr uint REGISTERSNUMBER = 6;

struct Register {
    int value;
    bool isUsed;
};

class Memory {
public:
    Memory();
    Variable getVariable(std::string name);
    void setVariable(std::string name);
    void setArray(std::string name, uint start, uint end);

    std::string getFreeRegister();
    void freeRegister(std::string reg);

private:
    void checkIfVariableNotExitsts(std::string name);
    void checkIfVariableAlreadyExists(std::string name);
    void checkArraySize(uint start, uint end);

    std::map<std::string, Variable> variables;
    std::vector<Register> registers;
    uint freeMemPtr;
};

#endif //MEMORY_H
