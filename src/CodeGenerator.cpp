#include "CodeGenerator.h"

CodeGenerator::CodeGenerator(Memory* memory) {
    this->memory = memory;
}

void CodeGenerator::readVariable(Variable* var) {
    var->initialize();
    uint address = var->getAddress();
    std::string reg = memory->getFreeRegister();

    this->setRegisterValue(reg, address);
    this->commands.push_back(new Command(GET, reg));

    memory->freeRegister(reg);
}

void CodeGenerator::writeVariable(Variable* var) {
    uint address = var->getAddress();
    std::string reg = memory->getFreeRegister();

    this->setRegisterValue(reg, address);
    this->commands.push_back(new Command(PUT, reg));
    
    memory->freeRegister(reg);
}

void CodeGenerator::setRegisterValue(std::string reg, uint value) {
    this->commands.push_back(new Command(RESET, reg));
    std::string binary = this->decToBin(value);

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

std::string CodeGenerator::decToBin(uint value) {
    std::string binary;
    while (value > 0) {
        binary = (value % 2 == 0 ? "0" : "1") + binary;
        value /= 2;
    }
    return binary;
}
