#ifndef CODEGENERATOR_H
#define CODEGENERATOR_H


#include <vector>
#include <stack>

#include "Command.h"
#include "Memory.h"

class CodeGenerator {
public:
    CodeGenerator(Memory* memory);

    void readVariable(Variable* var);
    void writeVariable(Variable* var);

    void setConstValue(Variable* var);

    void endGenerateCode();
    std::string getCode();

private:
    void setRegisterValue(std::string reg, uint value);
    std::string decToBin(uint value);

    Memory* memory;
    std::vector<Command*> commands;
    std::stack<Command*> stack;
};

#endif //CODEGENERATOR_H
