#ifndef MEMORY_H
#define MEMORY_H


#include <map>
#include <vector>

#include "variable/Variable.h"
#include "variable/Constant.h"
#include "variable/Iterator.h"

constexpr uint REGISTERSNUMBER = 6;

struct Register {
    int value;
    bool isUsed;
};

class Memory {
public:
    Memory();
    Variable* getVariable(std::string name);
    Variable* getArrayVariable(std::string name, uint index);
    Variable* getArrayVariable(std::string name, std::string index);
    std::pair<Variable*, bool> getConstant(uint value);

    void setVariable(std::string name);
    void setArray(std::string name, uint start, uint end);
    Iterator* setIterator(std::string name);
    void deleteIterator(Iterator* it);

    std::string getFreeRegister();
    std::pair<std::string, bool> getFreeRegister(Variable* var);
    void freeRegister(std::string reg, int value);

    void resetRegistersValue();
    void assertFreeRegisters();

private:
    void checkIfVariableNotExitsts(std::string name);
    void checkIfVariableAlreadyExists(std::string name);
    void checkIfTryUseArrayAsVariable(std::string name);
    void checkArraySize(uint start, uint end);

    std::map<std::string, Variable*> variables;
    std::vector<Constant*> constants;
    std::vector<Register> registers;
    uint freeMemPtr{0};
    uint lastSentRegNumber{5};
};

#endif //MEMORY_H
