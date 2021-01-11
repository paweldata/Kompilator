#include "CodeGenerator.h"

CodeGenerator::CodeGenerator(Memory* memory)
    : operations(*this)
    , conditions(*this)
    , flowControler(*this) {
    this->memory = memory;
}

void CodeGenerator::readVariable(Variable* var) {
    if (ArrayAddress* arrAddress = dynamic_cast<ArrayAddress*>(var)) {
        this->readArrayAddress(arrAddress);
        return;
    }

    var->initialize();
    uint address = var->getAddress();
    std::string reg = memory->getFreeRegister();

    this->setRegisterValue(reg, address);
    this->commands.push_back(new Command(GET, reg));

    memory->freeRegister(reg);
}

void CodeGenerator::writeVariable(Variable* var) {
    if (ArrayAddress* arrAddress = dynamic_cast<ArrayAddress*>(var)) {
        this->writeArrayAddress(arrAddress);
        return;
    }

    uint address = var->getAddress();
    std::string reg = memory->getFreeRegister();

    this->setRegisterValue(reg, address);
    this->commands.push_back(new Command(PUT, reg));

    memory->freeRegister(reg);
}

void CodeGenerator::setConstValue(Variable* var) {
    Constant* constant = dynamic_cast<Constant*>(var);
    uint address = constant->getAddress();
    uint value = constant->getValue();

    std::string regWithValue = memory->getFreeRegister();
    std::string regWithAddress = memory->getFreeRegister();
    std::string param = regWithValue + " " + regWithAddress;

    this->setRegisterValue(regWithValue, value);
    this->setRegisterValue(regWithAddress, address);
    this->commands.push_back(new Command(STORE, param));

    memory->freeRegister(regWithValue);
    memory->freeRegister(regWithAddress);
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

void CodeGenerator::assignValue(Variable* var, std::string reg) {
    if (ArrayAddress* arrAddress = dynamic_cast<ArrayAddress*>(var)) {
        this->assignArrayValue(arrAddress, reg);
        return;
    }

    var->initialize();
    uint address = var->getAddress();
    std::string varReg = memory->getFreeRegister();
    std::string param = reg + " " + varReg;

    this->setRegisterValue(varReg, address);
    this->commands.push_back(new Command(STORE, param));

    memory->freeRegister(reg);
    memory->freeRegister(varReg);
}

std::string* CodeGenerator::setVarToRegister(Variable* var) {
    if (ArrayAddress* arrAddress = dynamic_cast<ArrayAddress*>(var))
        return this->setArrVarToRegister(arrAddress);

    uint address = var->getAddress();
    std::string reg = memory->getFreeRegister();
    std::string param = reg + " " + reg;

    this->setRegisterValue(reg, address);
    this->commands.push_back(new Command(LOAD, param));
    return new std::string(reg);
}

void CodeGenerator::endGenerateCode() {
    this->commands.push_back(new Command(HALT, ""));
    this->memory->assertFreeRegisters();
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

void CodeGenerator::readArrayAddress(ArrayAddress* arr) {
    Variable* var = arr->getIndex();
    uint varAddress = var->getAddress();
    uint arrAddress = arr->getAddress();

    std::string regWithAddress = memory->getFreeRegister();
    std::string regWithIndexValue = memory->getFreeRegister();
    std::string param = regWithIndexValue + " " + regWithAddress;

    this->setRegisterValue(regWithAddress, varAddress);
    this->commands.push_back(new Command(LOAD, param));
    this->setRegisterValue(regWithAddress, arrAddress);
    this->commands.push_back(new Command(ADD, param));
    this->commands.push_back(new Command(GET, regWithIndexValue));

    memory->freeRegister(regWithAddress);
    memory->freeRegister(regWithIndexValue);
}

void CodeGenerator::writeArrayAddress(ArrayAddress* arr) {
    Variable* var = arr->getIndex();
    uint varAddress = var->getAddress();
    uint arrAddress = arr->getAddress();

    std::string regWithAddress = memory->getFreeRegister();
    std::string regWithIndexValue = memory->getFreeRegister();
    std::string param = regWithIndexValue + " " + regWithAddress;

    this->setRegisterValue(regWithAddress, varAddress);
    this->commands.push_back(new Command(LOAD, param));
    this->setRegisterValue(regWithAddress, arrAddress);
    this->commands.push_back(new Command(ADD, param));
    this->commands.push_back(new Command(PUT, regWithIndexValue));

    memory->freeRegister(regWithAddress);
    memory->freeRegister(regWithIndexValue);
}

void CodeGenerator::assignArrayValue(ArrayAddress* arr, std::string reg) {
    Variable* var = arr->getIndex();
    uint varAddress = var->getAddress();
    uint arrAddress = arr->getAddress();

    std::string regWithAddress = memory->getFreeRegister();
    std::string regWithIndexValue = memory->getFreeRegister();
    std::string param = regWithIndexValue + " " + regWithAddress;
    std::string storeParam = reg + " " + regWithIndexValue;

    this->setRegisterValue(regWithAddress, varAddress);
    this->commands.push_back(new Command(LOAD, param));
    this->setRegisterValue(regWithAddress, arrAddress);
    this->commands.push_back(new Command(ADD, param));
    this->commands.push_back(new Command(STORE, storeParam));

    memory->freeRegister(regWithAddress);
    memory->freeRegister(regWithIndexValue);
    memory->freeRegister(reg);
}

std::string* CodeGenerator::setArrVarToRegister(ArrayAddress* arr) {
    Variable* var = arr->getIndex();
    uint varAddress = var->getAddress();
    uint arrAddress = arr->getAddress();

    std::string regWithAddress = memory->getFreeRegister();
    std::string regWithIndexValue = memory->getFreeRegister();
    std::string param = regWithIndexValue + " " + regWithAddress;
    std::string loadParam = regWithIndexValue + " " + regWithIndexValue;

    this->setRegisterValue(regWithAddress, varAddress);
    this->commands.push_back(new Command(LOAD, param));
    this->setRegisterValue(regWithAddress, arrAddress);
    this->commands.push_back(new Command(ADD, param));
    this->commands.push_back(new Command(LOAD, loadParam));

    memory->freeRegister(regWithAddress);
    return new std::string(regWithIndexValue);
}
