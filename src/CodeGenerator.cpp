#include "CodeGenerator.h"

CodeGenerator::CodeGenerator(Memory* memory) {
    this->memory = memory;
}

void CodeGenerator::getVariable(std::string name) {
    Variable* var = this->memory->getVariable(name);
    this->getVarInRegisterAndGenerateCode(var);
}

void CodeGenerator::readVariable(std::string name) {
    Variable* var = this->memory->getVariable(name);
    var->initialize();
    uint address = var->getAddress();
    std::string reg = memory->getFreeRegister();

    this->setRegisterValue(reg, address);

    this->commands.push_back(new Command(GET, reg));

    memory->freeRegister(reg);
}

void CodeGenerator::writeVariable(std::string name) {
    Variable* var = this->memory->getVariable(name);
    uint address = var->getAddress();
    std::string reg = memory->getFreeRegister();

    this->setRegisterValue(reg, address);

    this->commands.push_back(new Command(PUT, reg));

    memory->freeRegister(reg);
}

void CodeGenerator::setRegisterValue(std::string reg, uint value) {
    this->commands.push_back(new Command(RESET, reg));

    std::string binary;

    while (value > 0) {
        binary = (value % 2 == 0 ? "0" : "1") + binary;
        value /= 2;
    }

    for (char c : binary) {
        this->commands.push_back(new Command(SHL, reg));
        if (c == '1')
            this->commands.push_back(new Command(INC, reg));
    }
}

void CodeGenerator::endGenerateCode() {
    this->commands.push_back(new Command(HALT, ""));
}

std::string CodeGenerator::getCode() {
    std::string code;
    for (Command* command : this->commands)
        code.append(command->parseToString() + "\n");
    return code;
}

int CodeGenerator::getVarInRegisterAndGenerateCode(Variable* variable) {
    // uint address = variable.getAddress();
    // return register and generate code
    return 0;
}
