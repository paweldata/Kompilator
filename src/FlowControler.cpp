#include "CodeGenerator.h"

void CodeGenerator::FlowControler::singleIf(Condition cond) {
    uint jumpValue = this->codeGen.commands.size() - cond.afterCondPtr + 1;
    cond.falseJump->setParam(std::to_string(jumpValue));
    this->codeGen.memory->resetRegistersValue();
}

IfElseParam CodeGenerator::FlowControler::ifElseFirst(Condition cond) {
    Command* jump = new Command(JUMP, "");
    this->codeGen.commands.push_back(jump);
    this->singleIf(cond);

    uint ptr = this->codeGen.commands.size();
    this->codeGen.memory->resetRegistersValue();
    return IfElseParam{.ptr = ptr, .jump = jump};
}

void CodeGenerator::FlowControler::ifElseSecond(IfElseParam param) {
    uint jumpValue = this->codeGen.commands.size() - param.ptr + 1;
    param.jump->setParam(std::to_string(jumpValue));
    this->codeGen.memory->resetRegistersValue();
}

void CodeGenerator::FlowControler::whileLoop(Condition cond) {
    int jumpValue = cond.beforeCondPtr - this->codeGen.commands.size();
    this->codeGen.commands.push_back(new Command(JUMP, std::to_string(jumpValue)));

    jumpValue = this->codeGen.commands.size() - cond.afterCondPtr + 1;
    cond.falseJump->setParam(std::to_string(jumpValue));
    this->codeGen.memory->resetRegistersValue();
}

uint CodeGenerator::FlowControler::repeatUntilFirst() {
    this->codeGen.memory->resetRegistersValue();
    return this->codeGen.commands.size();
}

void CodeGenerator::FlowControler::repeatUntilSecond(Condition cond, uint ptr) {
    int jumpValue = ptr - this->codeGen.commands.size() + 1;
    cond.falseJump->setParam(std::to_string(jumpValue));
    this->codeGen.memory->resetRegistersValue();
}

ForParam CodeGenerator::FlowControler::forLoopToFirst(std::string itName, Variable* firstValue, Variable* secondValue) {
    Iterator* it = this->codeGen.memory->setIterator(itName);
    std::string* reg1 = this->codeGen.setVarToRegister(firstValue);
    std::string* reg2 = this->codeGen.setVarToRegister(secondValue);

    std::string counterName = "COUNTER" + std::to_string(this->codeGen.commands.size());
    this->codeGen.memory->setVariable(counterName);
    Variable* counter = this->codeGen.memory->getVariable(counterName);

    this->codeGen.assignValueAfterChecks(it, *reg1);
    this->codeGen.commands.push_back(new Command(INC, *reg2));
    this->codeGen.commands.push_back(new Command(SUB, *reg2 + " " + *reg1));
    this->codeGen.assignValue(counter, *reg2);

    uint loopPtr = this->codeGen.commands.size();

    this->codeGen.memory->resetRegistersValue();

    uint address = counter->getAddress();
    std::string counterReg = this->codeGen.memory->getFreeRegister();
    std::string counterAdress = this->codeGen.memory->getFreeRegister();
    this->codeGen.setRegisterValue(counterAdress, address);
    this->codeGen.commands.push_back(new Command(LOAD, counterReg + " " + counterAdress));

    Command* jump = new Command(JUMP, "");
    this->codeGen.commands.push_back(new Command(JZERO, counterReg + " 2"));
    this->codeGen.commands.push_back(new Command(JUMP, "2"));
    this->codeGen.commands.push_back(jump);

    uint jumpPtr = this->codeGen.commands.size();

    this->codeGen.commands.push_back(new Command(DEC, counterReg));
    this->codeGen.commands.push_back(new Command(STORE, counterReg + " " + counterAdress));

    this->codeGen.memory->freeRegister(counterReg, -1);
    this->codeGen.memory->freeRegister(counterAdress, -1);

    return ForParam{.loopPtr = loopPtr, .jumpPtr = jumpPtr, .jump = jump, .it = it};
}

void CodeGenerator::FlowControler::forLoopToSecond(ForParam param) {
    uint address = param.it->getAddress();
    std::string itReg = this->codeGen.memory->getFreeRegister();
    std::string itAdress = this->codeGen.memory->getFreeRegister();

    this->codeGen.setRegisterValue(itAdress, address);
    this->codeGen.commands.push_back(new Command(LOAD, itReg + " " + itAdress));
    this->codeGen.commands.push_back(new Command(INC, itReg));
    this->codeGen.commands.push_back(new Command(STORE, itReg + " " + itAdress));

    this->codeGen.memory->freeRegister(itReg, -1);
    this->codeGen.memory->freeRegister(itAdress, -1);

    int jumpValue = param.loopPtr - this->codeGen.commands.size();
    this->codeGen.commands.push_back(new Command(JUMP, std::to_string(jumpValue)));

    jumpValue = this->codeGen.commands.size() - param.jumpPtr + 1;
    param.jump->setParam(std::to_string(jumpValue));

    this->codeGen.memory->deleteIterator(param.it);
    this->codeGen.memory->resetRegistersValue();
}

ForParam CodeGenerator::FlowControler::forLoopDownToFirst(std::string itName, Variable* firstValue, Variable* secondValue) {
    Iterator* it = this->codeGen.memory->setIterator(itName);
    std::string* reg1 = this->codeGen.setVarToRegister(firstValue);
    std::string* reg2 = this->codeGen.setVarToRegister(secondValue);

    std::string counterName = "COUNTER" + std::to_string(this->codeGen.commands.size());
    this->codeGen.memory->setVariable(counterName);
    Variable* counter = this->codeGen.memory->getVariable(counterName);
    std::string reg3 = this->codeGen.memory->getFreeRegister();

    this->codeGen.commands.push_back(new Command(RESET, reg3));
    this->codeGen.commands.push_back(new Command(ADD, reg3 + " " + *reg1));
    this->codeGen.assignValueAfterChecks(it, reg3);
    this->codeGen.commands.push_back(new Command(INC, *reg1));
    this->codeGen.commands.push_back(new Command(SUB, *reg1 + " " + *reg2));
    this->codeGen.assignValue(counter, *reg1);
    this->codeGen.memory->freeRegister(*reg2, -1);

    uint loopPtr = this->codeGen.commands.size();

    this->codeGen.memory->resetRegistersValue();

    uint address = counter->getAddress();
    std::string counterReg = this->codeGen.memory->getFreeRegister();
    std::string counterAdress = this->codeGen.memory->getFreeRegister();
    this->codeGen.setRegisterValue(counterAdress, address);
    this->codeGen.commands.push_back(new Command(LOAD, counterReg + " " + counterAdress));

    Command* jump = new Command(JUMP, "");
    this->codeGen.commands.push_back(new Command(JZERO, counterReg + " 2"));
    this->codeGen.commands.push_back(new Command(JUMP, "2"));
    this->codeGen.commands.push_back(jump);

    uint jumpPtr = this->codeGen.commands.size();

    this->codeGen.commands.push_back(new Command(DEC, counterReg));
    this->codeGen.commands.push_back(new Command(STORE, counterReg + " " + counterAdress));

    this->codeGen.memory->freeRegister(counterReg, -1);
    this->codeGen.memory->freeRegister(counterAdress, -1);

    return ForParam{.loopPtr = loopPtr, .jumpPtr = jumpPtr, .jump = jump, .it = it};
}

void CodeGenerator::FlowControler::forLoopDownToSecond(ForParam param) {
    uint address = param.it->getAddress();
    std::string itReg = this->codeGen.memory->getFreeRegister();
    std::string itAdress = this->codeGen.memory->getFreeRegister();

    this->codeGen.setRegisterValue(itAdress, address);
    this->codeGen.commands.push_back(new Command(LOAD, itReg + " " + itAdress));
    this->codeGen.commands.push_back(new Command(DEC, itReg));
    this->codeGen.commands.push_back(new Command(STORE, itReg + " " + itAdress));

    this->codeGen.memory->freeRegister(itReg, -1);
    this->codeGen.memory->freeRegister(itAdress, -1);

    int jumpValue = param.loopPtr - this->codeGen.commands.size();
    this->codeGen.commands.push_back(new Command(JUMP, std::to_string(jumpValue)));

    jumpValue = this->codeGen.commands.size() - param.jumpPtr + 1;
    param.jump->setParam(std::to_string(jumpValue));

    this->codeGen.memory->deleteIterator(param.it);
    this->codeGen.memory->resetRegistersValue();
}
