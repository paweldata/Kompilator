#include "CodeGenerator.h"

std::string* CodeGenerator::Operations::add(Variable* var1, Variable* var2) {
    std::string* reg1 = this->codeGen.setVarToRegister(var1);
    std::string* reg2 = this->codeGen.setVarToRegister(var2);
    std::string param = *reg1 + " " + *reg2;

    this->codeGen.commands.push_back(new Command(ADD, param));

    this->codeGen.memory->freeRegister(*reg2);
    return reg1;
}

std::string* CodeGenerator::Operations::sub(Variable* var1, Variable* var2) {
    std::string* reg1 = this->codeGen.setVarToRegister(var1);
    std::string* reg2 = this->codeGen.setVarToRegister(var2);
    std::string param = *reg1 + " " + *reg2;

    this->codeGen.commands.push_back(new Command(SUB, param));

    this->codeGen.memory->freeRegister(*reg2);
    return reg1;
}
