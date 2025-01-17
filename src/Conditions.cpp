#include "CodeGenerator.h"

Condition CodeGenerator::Conditions::equal(Variable* var1, Variable* var2) {
    this->codeGen.memory->resetRegistersValue();

    uint beforeCondPtr = this->codeGen.commands.size();
    Command* falseJump = new Command(JUMP, "");

    std::string* reg1 = this->codeGen.setVarToRegister(var1);
    std::string* reg2 = this->codeGen.setVarToRegister(var2);
    std::string checkReg = this->codeGen.getRegisterWithValue(0);

    std::string var1Param = checkReg + " " + *reg1;
    std::string var2Param = checkReg + " " + *reg2;
    std::string jzeroParam = checkReg + " 2";

    this->codeGen.commands.push_back(new Command(ADD, var1Param));
    this->codeGen.commands.push_back(new Command(SUB, var2Param));
    this->codeGen.commands.push_back(new Command(JZERO, jzeroParam));
    this->codeGen.commands.push_back(new Command(JUMP, "4"));
    this->codeGen.commands.push_back(new Command(ADD, var2Param));
    this->codeGen.commands.push_back(new Command(SUB, var1Param));
    this->codeGen.commands.push_back(new Command(JZERO, jzeroParam));
    this->codeGen.commands.push_back(falseJump);

    uint afterCondPtr = this->codeGen.commands.size();
    Condition cond{
        .beforeCondPtr = beforeCondPtr,
        .afterCondPtr = afterCondPtr,
        .falseJump = falseJump};

    this->codeGen.memory->freeRegister(*reg1, -1);
    this->codeGen.memory->freeRegister(*reg2, -1);
    this->codeGen.memory->freeRegister(checkReg, -1);

    return cond;
}

Condition CodeGenerator::Conditions::notEqual(Variable* var1, Variable* var2) {
    Condition cond = this->equal(var1, var2);
    cond.falseJump->setParam("2");

    cond.falseJump = new Command(JUMP, "");
    this->codeGen.commands.push_back(cond.falseJump);
    cond.afterCondPtr++;
    return cond;
}

Condition CodeGenerator::Conditions::less(Variable* var1, Variable* var2) {
    this->codeGen.memory->resetRegistersValue();

    uint beforeCondPtr = this->codeGen.commands.size();
    Command* falseJump = new Command(JUMP, "");

    std::string* reg1 = this->codeGen.setVarToRegister(var1);
    std::string* reg2 = this->codeGen.setVarToRegister(var2);
    std::string checkReg = this->codeGen.getRegisterWithValue(0);

    std::string var1Param = checkReg + " " + *reg1;
    std::string var2Param = checkReg + " " + *reg2;
    std::string jzeroParam = checkReg + " 2";

    this->codeGen.commands.push_back(new Command(ADD, var2Param));
    this->codeGen.commands.push_back(new Command(SUB, var1Param));
    this->codeGen.commands.push_back(new Command(JZERO, jzeroParam));
    this->codeGen.commands.push_back(new Command(JUMP, "2"));
    this->codeGen.commands.push_back(falseJump);

    uint afterCondPtr = this->codeGen.commands.size();
    Condition cond{
        .beforeCondPtr = beforeCondPtr,
        .afterCondPtr = afterCondPtr,
        .falseJump = falseJump};

    this->codeGen.memory->freeRegister(*reg1, -1);
    this->codeGen.memory->freeRegister(*reg2, -1);
    this->codeGen.memory->freeRegister(checkReg, -1);

    return cond;
}

Condition CodeGenerator::Conditions::lessOrEqual(Variable* var1, Variable* var2) {
    this->codeGen.memory->resetRegistersValue();

    uint beforeCondPtr = this->codeGen.commands.size();
    Command* falseJump = new Command(JUMP, "");

    std::string* reg1 = this->codeGen.setVarToRegister(var1);
    std::string* reg2 = this->codeGen.setVarToRegister(var2);
    std::string checkReg = this->codeGen.getRegisterWithValue(0);

    std::string var1Param = checkReg + " " + *reg1;
    std::string var2Param = checkReg + " " + *reg2;
    std::string jzeroParam = checkReg + " 2";

    this->codeGen.commands.push_back(new Command(ADD, var1Param));
    this->codeGen.commands.push_back(new Command(SUB, var2Param));
    this->codeGen.commands.push_back(new Command(JZERO, jzeroParam));
    this->codeGen.commands.push_back(falseJump);

    uint afterCondPtr = this->codeGen.commands.size();
    Condition cond{
        .beforeCondPtr = beforeCondPtr,
        .afterCondPtr = afterCondPtr,
        .falseJump = falseJump};

    this->codeGen.memory->freeRegister(*reg1, -1);
    this->codeGen.memory->freeRegister(*reg2, -1);
    this->codeGen.memory->freeRegister(checkReg, -1);

    return cond;
}

Condition CodeGenerator::Conditions::greater(Variable* var1, Variable* var2) {
    Condition cond = this->lessOrEqual(var1, var2);
    cond.falseJump->setParam("2");

    cond.falseJump = new Command(JUMP, "");
    this->codeGen.commands.push_back(cond.falseJump);
    cond.afterCondPtr++;
    return cond;
}

Condition CodeGenerator::Conditions::greaterOrEqual(Variable* var1, Variable* var2) {
    Condition cond = this->less(var1, var2);
    cond.falseJump->setParam("2");

    cond.falseJump = new Command(JUMP, "");
    this->codeGen.commands.push_back(cond.falseJump);
    cond.afterCondPtr++;
    return cond;
}
