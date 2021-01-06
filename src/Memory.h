#ifndef MEMORY_H
#define MEMORY_H


#include <map>

#include "Variable.h"

class Memory {
public:
    Memory();
    Variable getVariable(std::string name);
    void setVariable(std::string name);
    void setArray(std::string name, uint start, uint end);

private:
    void checkIfVariableNotExitsts(std::string name);
    void checkIfVariableAlreadyExists(std::string name);
    void checkArraySize(uint start, uint end);

    std::map<std::string, Variable> variables;
    uint freeMemPtr;
};

#endif //MEMORY_H
