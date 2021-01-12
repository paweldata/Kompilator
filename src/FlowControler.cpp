#include "CodeGenerator.h"

void CodeGenerator::FlowControler::singleIf(Condition cond) {
    uint jumpValue = this->codeGen.commands.size() - cond.afterCondPtr + 1;
    cond.falseJump->setParam(std::to_string(jumpValue));
}
