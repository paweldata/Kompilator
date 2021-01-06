#include "CodeGenerator.h"

CodeGenerator::CodeGenerator(Memory* memory) {
    this->memory = memory;
}

void CodeGenerator::getVariable(std::string name) {
    Variable var = this->memory->getVariable(name);
    this->getVarInRegisterAndGenerateCode(var);
}

int CodeGenerator::getVarInRegisterAndGenerateCode(Variable variable) {
    // uint address = variable.getAddress();
    // return register and generate code
    return 0;
}
