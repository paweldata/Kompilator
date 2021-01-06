#ifndef CODEGENERATOR_H
#define CODEGENERATOR_H


#include <vector>
#include <stack>

#include "Command.h"
#include "Memory.h"

class CodeGenerator {
public:
    CodeGenerator(Memory* memory);

private:
    Memory* memory;
    std::vector<Command*> commands;
    std::stack<Command*> stack;
};

#endif //CODEGENERATOR_H
