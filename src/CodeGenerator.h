#ifndef CODEGENERATOR_H
#define CODEGENERATOR_H


#include <vector>
#include <stack>

#include "Command.h"
#include "Memory.h"

class CodeGenerator {
public:
    CodeGenerator(Memory* memory);
    void getVariable(std::string name);

private:
    int getVarInRegisterAndGenerateCode(Variable variable);

    Memory* memory;
    std::vector<Command*> commands;
    std::stack<Command*> stack;
};

#endif //CODEGENERATOR_H
