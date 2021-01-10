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

std::string* CodeGenerator::Operations::div(Variable* var1, Variable* var2) {
    std::string counterReg = this->codeGen.memory->getFreeRegister();
    std::string reg1 = this->codeGen.memory->getFreeRegister();
    std::string* reg2 = this->codeGen.setVarToRegister(var1);

    this->codeGen.commands.push_back(new Command(RESET, counterReg));
    this->codeGen.commands.push_back(new Command(RESET, reg1));

    std::string jzeroParam = *reg2 + " 8";
    std::string joddParam = *reg2 + " 2";

    // first loop, inverse divisor
    this->codeGen.commands.push_back(new Command(JZERO, jzeroParam));
    this->codeGen.commands.push_back(new Command(SHL, reg1));
    this->codeGen.commands.push_back(new Command(JODD, joddParam));
    this->codeGen.commands.push_back(new Command(JUMP, "2"));
    this->codeGen.commands.push_back(new Command(INC, reg1));
    this->codeGen.commands.push_back(new Command(SHR, *reg2));
    this->codeGen.commands.push_back(new Command(INC, counterReg));
    this->codeGen.commands.push_back(new Command(JUMP, "-7"));
    // end first loop

    this->codeGen.memory->freeRegister(*reg2);
    reg2 = this->codeGen.setVarToRegister(var2);

    std::string tempSumReg = this->codeGen.memory->getFreeRegister();
    std::string resultReg = this->codeGen.memory->getFreeRegister();
    std::string checkReg = this->codeGen.memory->getFreeRegister();

    this->codeGen.commands.push_back(new Command(RESET, tempSumReg));
    this->codeGen.commands.push_back(new Command(RESET, resultReg));

    std::string jzeroParam1 = counterReg + " 16";
    joddParam = reg1 + " 2";

    // second loop, divide
    this->codeGen.commands.push_back(new Command(JZERO, jzeroParam1));
    this->codeGen.commands.push_back(new Command(DEC, counterReg));
    this->codeGen.commands.push_back(new Command(SHL, tempSumReg));
    this->codeGen.commands.push_back(new Command(SHL, resultReg));
    this->codeGen.commands.push_back(new Command(JODD, joddParam));
    this->codeGen.commands.push_back(new Command(JUMP, "2"));
    this->codeGen.commands.push_back(new Command(INC, tempSumReg));
    this->codeGen.commands.push_back(new Command(SHR, reg1));

    std::string addParam = checkReg + " " + *reg2;
    std::string subParam1 = checkReg + " " + tempSumReg;
    std::string jzeroParam2 = checkReg + " 2";
    std::string subParam2 = tempSumReg + " " + *reg2;

    // check, if tempSum >= divident
    this->codeGen.commands.push_back(new Command(RESET, checkReg));
    this->codeGen.commands.push_back(new Command(ADD, addParam));
    this->codeGen.commands.push_back(new Command(SUB, subParam1));
    this->codeGen.commands.push_back(new Command(JZERO, jzeroParam2));
    this->codeGen.commands.push_back(new Command(JUMP, "3"));
    this->codeGen.commands.push_back(new Command(SUB, subParam2));
    this->codeGen.commands.push_back(new Command(INC, resultReg));

    this->codeGen.commands.push_back(new Command(JUMP, "-15"));
    // end second loop

    this->codeGen.memory->freeRegister(reg1);
    this->codeGen.memory->freeRegister(*reg2);
    this->codeGen.memory->freeRegister(counterReg);
    this->codeGen.memory->freeRegister(tempSumReg);
    this->codeGen.memory->freeRegister(checkReg);
    return new std::string(resultReg);
}

std::string* CodeGenerator::Operations::mod(Variable* var1, Variable* var2) {
    std::string* reg1 = this->codeGen.setVarToRegister(var1);
    std::string* reg2 = this->codeGen.setVarToRegister(var2);
    std::string checkReg = this->codeGen.memory->getFreeRegister();

    std::string checkModZero = *reg2 + " 8";
    std::string addParam = checkReg + " " + *reg2;
    std::string subParam = checkReg + " " + *reg1;
    std::string checkEnd = checkReg + " 2";
    std::string subAandB = *reg1 + " " + *reg2;

    this->codeGen.commands.push_back(new Command(JZERO, checkModZero));
    this->codeGen.commands.push_back(new Command(RESET, checkReg));
    this->codeGen.commands.push_back(new Command(ADD, addParam));
    this->codeGen.commands.push_back(new Command(SUB, subParam));
    this->codeGen.commands.push_back(new Command(JZERO, checkEnd));
    this->codeGen.commands.push_back(new Command(JUMP, "4"));
    this->codeGen.commands.push_back(new Command(SUB, subAandB));
    this->codeGen.commands.push_back(new Command(JUMP, "-6"));
    this->codeGen.commands.push_back(new Command(RESET, *reg1));

    this->codeGen.memory->freeRegister(*reg2);
    this->codeGen.memory->freeRegister(checkReg);
    return reg1;
}
