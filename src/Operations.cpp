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

std::string* CodeGenerator::Operations::mul(Variable* var1, Variable* var2) {
    std::string* reg1 = this->codeGen.setVarToRegister(var1);
    std::string* reg2 = this->codeGen.setVarToRegister(var2);
    std::string resultReg = this->codeGen.memory->getFreeRegister();

    std::string jzeroParam = *reg2 + " 7";
    std::string joddParam = *reg2 + " 2";
    std::string addParam = resultReg + " " + *reg1;

    this->codeGen.commands.push_back(new Command(RESET, resultReg));
    this->codeGen.commands.push_back(new Command(JZERO, jzeroParam));
    this->codeGen.commands.push_back(new Command(JODD, joddParam));
    this->codeGen.commands.push_back(new Command(JUMP, "2"));
    this->codeGen.commands.push_back(new Command(ADD, addParam));
    this->codeGen.commands.push_back(new Command(SHL, *reg1));
    this->codeGen.commands.push_back(new Command(SHR, *reg2));
    this->codeGen.commands.push_back(new Command(JUMP, "-6"));

    this->codeGen.memory->freeRegister(*reg2);
    this->codeGen.memory->freeRegister(*reg1);
    return new std::string(resultReg);
}
