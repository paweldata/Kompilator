#include "CodeGenerator.h"

void CodeGenerator::FlowControler::singleIf(Condition cond) {
    uint jumpValue = this->codeGen.commands.size() - cond.afterCondPtr + 1;
    cond.falseJump->setParam(std::to_string(jumpValue));
}

IfElseParam CodeGenerator::FlowControler::ifElseFirst(Condition cond) {
    Command* jump = new Command(JUMP, "");
    this->codeGen.commands.push_back(jump);

    this->singleIf(cond);

    uint ptr = this->codeGen.commands.size();
    return IfElseParam{.ptr = ptr, .jump = jump};
}

void CodeGenerator::FlowControler::ifElseSecond(IfElseParam param) {
    uint jumpValue = this->codeGen.commands.size() - param.ptr + 1;
    param.jump->setParam(std::to_string(jumpValue));
}

void CodeGenerator::FlowControler::whileLoop(Condition cond) {
    int jumpValue = cond.beforeCondPtr - this->codeGen.commands.size();
    this->codeGen.commands.push_back(new Command(JUMP, std::to_string(jumpValue)));

    jumpValue = this->codeGen.commands.size() - cond.afterCondPtr + 1;
    cond.falseJump->setParam(std::to_string(jumpValue));
}
