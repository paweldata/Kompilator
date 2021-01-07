#ifndef CODEGENERATOR_H
#define CODEGENERATOR_H


#include <vector>
#include <stack>

#include "Command.h"
#include "Memory.h"

class CodeGenerator {
public:
    CodeGenerator(Memory* memory);

    void readVariable(std::string name);
    void writeVariable(std::string name);

    void endGenerateCode();
    std::string getCode();

private:
    void setRegisterValue(std::string reg, uint value);

    Memory* memory;
    std::vector<Command*> commands;
    std::stack<Command*> stack;
};

#endif //CODEGENERATOR_H
