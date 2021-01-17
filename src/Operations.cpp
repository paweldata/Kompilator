#include "CodeGenerator.h"

std::string* CodeGenerator::Operations::add(Variable* var1, Variable* var2) {
    std::string* reg1 = this->codeGen.setVarToRegister(var1);
    std::string* reg2 = this->codeGen.setVarToRegister(var2);

    this->codeGen.commands.push_back(new Command(ADD, *reg1 + " " + *reg2));
    this->codeGen.memory->freeRegister(*reg2, -1);
    return reg1;
}

std::string* CodeGenerator::Operations::sub(Variable* var1, Variable* var2) {
    std::string* reg1 = this->codeGen.setVarToRegister(var1);
    std::string* reg2 = this->codeGen.setVarToRegister(var2);

    this->codeGen.commands.push_back(new Command(SUB, *reg1 + " " + *reg2));
    this->codeGen.memory->freeRegister(*reg2, -1);
    return reg1;
}

std::string* CodeGenerator::Operations::mul(Variable* var1, Variable* var2) {
    std::string* reg1 = this->codeGen.setVarToRegister(var1);
    std::string* reg2 = this->codeGen.setVarToRegister(var2);
    std::string resultReg = this->codeGen.getRegisterWithValue(0);

    // find lower number
    this->codeGen.commands.push_back(new Command(ADD, resultReg + " " + *reg1));
    this->codeGen.commands.push_back(new Command(SUB, resultReg + " " + *reg2));
    this->codeGen.commands.push_back(new Command(JZERO, resultReg + " 9"));

    // var1 > var2
    this->codeGen.commands.push_back(new Command(RESET, resultReg));
    this->codeGen.commands.push_back(new Command(JZERO, *reg2 + " 14"));
    this->codeGen.commands.push_back(new Command(JODD, *reg2 + " 2"));
    this->codeGen.commands.push_back(new Command(JUMP, "2"));
    this->codeGen.commands.push_back(new Command(ADD, resultReg + " " + *reg1));
    this->codeGen.commands.push_back(new Command(SHL, *reg1));
    this->codeGen.commands.push_back(new Command(SHR, *reg2));
    this->codeGen.commands.push_back(new Command(JUMP, "-6"));

    // var1 <= var2
    this->codeGen.commands.push_back(new Command(JZERO, *reg1 + " 7"));
    this->codeGen.commands.push_back(new Command(JODD, *reg1 + " 2"));
    this->codeGen.commands.push_back(new Command(JUMP, "2"));
    this->codeGen.commands.push_back(new Command(ADD, resultReg + " " + *reg2));
    this->codeGen.commands.push_back(new Command(SHL, *reg2));
    this->codeGen.commands.push_back(new Command(SHR, *reg1));
    this->codeGen.commands.push_back(new Command(JUMP, "-6"));

    this->codeGen.memory->freeRegister(*reg2, -1);
    this->codeGen.memory->freeRegister(*reg1, -1);
    return new std::string(resultReg);
}

std::string* CodeGenerator::Operations::div(Variable* var1, Variable* var2) {
    std::string counterReg = this->codeGen.getRegisterWithValue(0);
    std::string reg1 = this->codeGen.getRegisterWithValue(0);
    std::string* reg2 = this->codeGen.setVarToRegister(var1);

    // first loop, inverse divisor
    this->codeGen.commands.push_back(new Command(JZERO, *reg2 + " 8"));
    this->codeGen.commands.push_back(new Command(SHL, reg1));
    this->codeGen.commands.push_back(new Command(JODD, *reg2 + " 2"));
    this->codeGen.commands.push_back(new Command(JUMP, "2"));
    this->codeGen.commands.push_back(new Command(INC, reg1));
    this->codeGen.commands.push_back(new Command(SHR, *reg2));
    this->codeGen.commands.push_back(new Command(INC, counterReg));
    this->codeGen.commands.push_back(new Command(JUMP, "-7"));
    // end first loop

    this->codeGen.memory->freeRegister(*reg2, -1);
    reg2 = this->codeGen.setVarToRegister(var2);

    std::string resultReg = this->codeGen.getRegisterWithValue(0);
    std::string tempSumReg = this->codeGen.getRegisterWithValue(0);
    std::string checkReg = this->codeGen.memory->getFreeRegister();

    // if var2 == 0, return 0
    this->codeGen.commands.push_back(new Command(JZERO, *reg2 + " 2"));
    this->codeGen.commands.push_back(new Command(JUMP, "2"));
    this->codeGen.commands.push_back(new Command(JUMP, "17"));

    // second loop, divide
    this->codeGen.commands.push_back(new Command(JZERO, counterReg + " 16"));
    this->codeGen.commands.push_back(new Command(DEC, counterReg));
    this->codeGen.commands.push_back(new Command(SHL, tempSumReg));
    this->codeGen.commands.push_back(new Command(SHL, resultReg));
    this->codeGen.commands.push_back(new Command(JODD, reg1 + " 2"));
    this->codeGen.commands.push_back(new Command(JUMP, "2"));
    this->codeGen.commands.push_back(new Command(INC, tempSumReg));
    this->codeGen.commands.push_back(new Command(SHR, reg1));

    // check, if tempSum >= divident
    this->codeGen.commands.push_back(new Command(RESET, checkReg));
    this->codeGen.commands.push_back(new Command(ADD, checkReg + " " + *reg2));
    this->codeGen.commands.push_back(new Command(SUB, checkReg + " " + tempSumReg));
    this->codeGen.commands.push_back(new Command(JZERO, checkReg + " 2"));
    this->codeGen.commands.push_back(new Command(JUMP, "3"));
    this->codeGen.commands.push_back(new Command(SUB, tempSumReg + " " + *reg2));
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
    std::string mulResultReg = this->codeGen.getRegisterWithValue(0);

    std::string checkZero = *reg2 + " 2";
    Command* jump = new Command(JUMP, "");
    Command* reset = new Command(RESET, "");

    // if var2 == 0, return 0
    this->codeGen.commands.push_back(new Command(JZERO, checkZero));
    this->codeGen.commands.push_back(new Command(JUMP, "3"));
    this->codeGen.commands.push_back(reset);
    this->codeGen.commands.push_back(jump);

    uint jumpPtr = this->codeGen.commands.size();

    // find lower number
    this->codeGen.commands.push_back(new Command(ADD, mulResultReg + " " + *reg1));
    this->codeGen.commands.push_back(new Command(SUB, mulResultReg + " " + *reg2));
    this->codeGen.commands.push_back(new Command(JZERO, mulResultReg + " 9"));

    // var1 > var2
    this->codeGen.commands.push_back(new Command(RESET, mulResultReg));
    this->codeGen.commands.push_back(new Command(JZERO, *reg2 + " 14"));
    this->codeGen.commands.push_back(new Command(JODD, *reg2 + " 2"));
    this->codeGen.commands.push_back(new Command(JUMP, "2"));
    this->codeGen.commands.push_back(new Command(ADD, mulResultReg + " " + *reg1));
    this->codeGen.commands.push_back(new Command(SHL, *reg1));
    this->codeGen.commands.push_back(new Command(SHR, *reg2));
    this->codeGen.commands.push_back(new Command(JUMP, "-6"));

    // var1 <= var2
    this->codeGen.commands.push_back(new Command(JZERO, *reg1 + " 7"));
    this->codeGen.commands.push_back(new Command(JODD, *reg1 + " 2"));
    this->codeGen.commands.push_back(new Command(JUMP, "2"));
    this->codeGen.commands.push_back(new Command(ADD, mulResultReg + " " + *reg2));
    this->codeGen.commands.push_back(new Command(SHL, *reg2));
    this->codeGen.commands.push_back(new Command(SHR, *reg1));
    this->codeGen.commands.push_back(new Command(JUMP, "-6"));

    this->codeGen.memory->freeRegister(*reg1, -1);
    reg1 = this->codeGen.setVarToRegister(var1);
    reset->setParam(*reg1);

    this->codeGen.commands.push_back(new Command(SUB, *reg1 + " " + mulResultReg));

    uint jumpSize = this->codeGen.commands.size() - jumpPtr + 1;
    jump->setParam(std::to_string(jumpSize));

    this->codeGen.memory->freeRegister(*reg2, -1);
    this->codeGen.memory->freeRegister(mulResultReg, -1);

    return reg1;
}
