#include "CodeGenerator.h"

std::string* CodeGenerator::Operations::add(Variable* var1, Variable* var2) {
    std::string* reg1 = this->codeGen.setVarToRegister(var1);
    std::string* reg2 = this->codeGen.setVarToRegister(var2);
    std::string param = *reg1 + " " + *reg2;

    this->codeGen.commands.push_back(new Command(ADD, param));

    this->codeGen.memory->freeRegister(*reg2, -1);
    return reg1;
}

std::string* CodeGenerator::Operations::sub(Variable* var1, Variable* var2) {
    std::string* reg1 = this->codeGen.setVarToRegister(var1);
    std::string* reg2 = this->codeGen.setVarToRegister(var2);
    std::string param = *reg1 + " " + *reg2;

    this->codeGen.commands.push_back(new Command(SUB, param));

    this->codeGen.memory->freeRegister(*reg2, -1);
    return reg1;
}

std::string* CodeGenerator::Operations::mul(Variable* var1, Variable* var2) {
    std::string* reg1 = this->codeGen.setVarToRegister(var1);
    std::string* reg2 = this->codeGen.setVarToRegister(var2);
    std::string resultReg = this->codeGen.getRegisterWithValue(0);

    std::string jzeroParam = *reg2 + " 7";
    std::string joddParam = *reg2 + " 2";
    std::string addParam = resultReg + " " + *reg1;

    this->codeGen.commands.push_back(new Command(JZERO, jzeroParam));
    this->codeGen.commands.push_back(new Command(JODD, joddParam));
    this->codeGen.commands.push_back(new Command(JUMP, "2"));
    this->codeGen.commands.push_back(new Command(ADD, addParam));
    this->codeGen.commands.push_back(new Command(SHL, *reg1));
    this->codeGen.commands.push_back(new Command(SHR, *reg2));
    this->codeGen.commands.push_back(new Command(JUMP, "-6"));

    this->codeGen.memory->freeRegister(*reg2, 0);
    this->codeGen.memory->freeRegister(*reg1, -1);
    return new std::string(resultReg);
}

std::string* CodeGenerator::Operations::div(Variable* var1, Variable* var2) {
    std::string counterReg = this->codeGen.getRegisterWithValue(0);
    std::string reg1 = this->codeGen.getRegisterWithValue(0);
    std::string* reg2 = this->codeGen.setVarToRegister(var1);
    std::string resultReg = this->codeGen.getRegisterWithValue(0);
    std::string tempSumReg = this->codeGen.getRegisterWithValue(0);
    std::string checkReg = this->codeGen.memory->getFreeRegister();

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

    this->codeGen.memory->freeRegister(*reg2, -1);
    reg2 = this->codeGen.setVarToRegister(var2);

    std::string checkZero = *reg2 + " 2";

    // if var2 == 0, return 0
    this->codeGen.commands.push_back(new Command(JZERO, checkZero));
    this->codeGen.commands.push_back(new Command(JUMP, "2"));
    this->codeGen.commands.push_back(new Command(JUMP, "17"));

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

    this->codeGen.memory->freeRegister(reg1, -1);
    this->codeGen.memory->freeRegister(*reg2, -1);
    this->codeGen.memory->freeRegister(counterReg, -1);
    this->codeGen.memory->freeRegister(tempSumReg, -1);
    this->codeGen.memory->freeRegister(checkReg, -1);
    return new std::string(resultReg);
}

std::string* CodeGenerator::Operations::mod(Variable* var1, Variable* var2) {
    // a % b = a - (b * int(a/b))
    std::string* reg1 = this->div(var1, var2);
    std::string *reg2 = this->codeGen.setVarToRegister(var2);
    std::string mulResultReg = this->codeGen.memory->getFreeRegister();

    std::string checkZero = *reg2 + " 2";
    Command* jump = new Command(JUMP, "");
    Command* reset = new Command(RESET, "");

    // if var2 == 0, return 0
    this->codeGen.commands.push_back(new Command(JZERO, checkZero));
    this->codeGen.commands.push_back(new Command(JUMP, "3"));
    this->codeGen.commands.push_back(reset);
    this->codeGen.commands.push_back(jump);

    uint jumpPtr = this->codeGen.commands.size();
    std::string jzeroParam = *reg2 + " 7";
    std::string joddParam = *reg2 + " 2";
    std::string addParam = mulResultReg + " " + *reg1;

    this->codeGen.commands.push_back(new Command(RESET, mulResultReg));
    this->codeGen.commands.push_back(new Command(JZERO, jzeroParam));
    this->codeGen.commands.push_back(new Command(JODD, joddParam));
    this->codeGen.commands.push_back(new Command(JUMP, "2"));
    this->codeGen.commands.push_back(new Command(ADD, addParam));
    this->codeGen.commands.push_back(new Command(SHL, *reg1));
    this->codeGen.commands.push_back(new Command(SHR, *reg2));
    this->codeGen.commands.push_back(new Command(JUMP, "-6"));

    this->codeGen.memory->freeRegister(*reg1, -1);
    reg1 = this->codeGen.setVarToRegister(var1);
    reset->setParam(*reg1);

    std::string subParam = *reg1 + " " + mulResultReg;

    this->codeGen.commands.push_back(new Command(SUB, subParam));

    uint jumpSize = this->codeGen.commands.size() - jumpPtr + 1;
    jump->setParam(std::to_string(jumpSize));

    this->codeGen.memory->freeRegister(*reg2, -1);
    this->codeGen.memory->freeRegister(mulResultReg, -1);

    return reg1;
}
