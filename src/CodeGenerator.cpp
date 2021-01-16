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
    std::string reg = this->getRegisterWithAddress(var);
    this->commands.push_back(new Command(GET, reg));
    memory->freeRegister(reg, var->getAddress());
}

void CodeGenerator::writeVariable(Variable* var) {
    if (ArrayAddress* arrAddress = dynamic_cast<ArrayAddress*>(var)) {
        this->writeArrayAddress(arrAddress);
        return;
    }
    std::string reg = this->getRegisterWithAddress(var);
    this->commands.push_back(new Command(PUT, reg));
    memory->freeRegister(reg, var->getAddress());
}

Variable* CodeGenerator::getConstant(uint value) {
    Variable* constant;
    bool isAlreadySet;
    std::tie(constant, isAlreadySet) = this->memory->getConstant(value);
    if (not isAlreadySet)
        this->setConstValue(constant);
    return constant;
}

void CodeGenerator::setConstValue(Variable* var) {
    Constant* constant = dynamic_cast<Constant*>(var);
    std::string regWithAddress = this->getRegisterWithAddress(var);
    std::string regWithValue = memory->getFreeRegister();

    this->setRegisterValue(regWithValue, constant->getValue());
    this->commands.push_back(new Command(STORE, regWithValue + " " + regWithAddress));

    memory->freeRegister(regWithValue, -1);
    memory->freeRegister(regWithAddress, constant->getAddress());
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

    this->checkIfTryModifyIterator(var);
    this->assignValueAfterChecks(var, reg);
}

void CodeGenerator::assignValueAfterChecks(Variable* var, std::string reg) {
    var->initialize();
    std::string varReg = this->getRegisterWithAddress(var);
    this->commands.push_back(new Command(STORE, reg + " " + varReg));

    memory->freeRegister(reg, -1);
    memory->freeRegister(varReg, var->getAddress());
}

std::string* CodeGenerator::setVarToRegister(Variable* var) {
    if (ArrayAddress* arrAddress = dynamic_cast<ArrayAddress*>(var))
        return this->setArrVarToRegister(arrAddress);

    std::string reg = this->getRegisterWithAddress(var);
    this->commands.push_back(new Command(LOAD, reg + " " + reg));
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

std::string CodeGenerator::getRegisterWithAddress(Variable* var) {
    std::string reg;
    bool isAlreadySet;
    std::tie(reg, isAlreadySet) = this->memory->getFreeRegister(var);
    if (not isAlreadySet)
        this->setRegisterValue(reg, var->getAddress());
    return reg;
}

void CodeGenerator::readArrayAddress(ArrayAddress* arr) {
    Variable* var = arr->getIndex();

    std::string regWithArrAddress = this->getRegisterWithAddress(arr);
    std::string regWithIndexAddress =  this->getRegisterWithAddress(var);
    std::string regWithSumAddress = this->memory->getFreeRegister();

    this->commands.push_back(new Command(RESET, regWithSumAddress));
    this->commands.push_back(new Command(LOAD, regWithSumAddress + " " + regWithIndexAddress));
    this->commands.push_back(new Command(ADD, regWithSumAddress + " " + regWithArrAddress));
    this->commands.push_back(new Command(GET, regWithSumAddress));

    memory->freeRegister(regWithArrAddress, arr->getAddress());
    memory->freeRegister(regWithIndexAddress, var->getAddress());
    memory->freeRegister(regWithSumAddress, -1);
}

void CodeGenerator::writeArrayAddress(ArrayAddress* arr) {
    Variable* var = arr->getIndex();

    std::string regWithArrAddress = this->getRegisterWithAddress(arr);
    std::string regWithIndexAddress =  this->getRegisterWithAddress(var);
    std::string regWithSumAddress = this->memory->getFreeRegister();

    this->commands.push_back(new Command(RESET, regWithSumAddress));
    this->commands.push_back(new Command(LOAD, regWithSumAddress + " " + regWithIndexAddress));
    this->commands.push_back(new Command(ADD, regWithSumAddress + " " + regWithArrAddress));
    this->commands.push_back(new Command(PUT, regWithSumAddress));

    memory->freeRegister(regWithArrAddress, arr->getAddress());
    memory->freeRegister(regWithIndexAddress, var->getAddress());
    memory->freeRegister(regWithSumAddress, -1);
}

void CodeGenerator::assignArrayValue(ArrayAddress* arr, std::string reg) {
    Variable* var = arr->getIndex();

    std::string regWithArrAddress = this->getRegisterWithAddress(arr);
    std::string regWithIndexAddress =  this->getRegisterWithAddress(var);
    std::string regWithSumAddress = this->memory->getFreeRegister();

    this->commands.push_back(new Command(RESET, regWithSumAddress));
    this->commands.push_back(new Command(LOAD, regWithSumAddress + " " + regWithIndexAddress));
    this->commands.push_back(new Command(ADD, regWithSumAddress + " " + regWithArrAddress));
    this->commands.push_back(new Command(STORE, reg + " " + regWithSumAddress));

    memory->freeRegister(regWithArrAddress, arr->getAddress());
    memory->freeRegister(regWithIndexAddress, var->getAddress());
    memory->freeRegister(regWithSumAddress, -1);
    memory->freeRegister(reg, -1);
}

std::string* CodeGenerator::setArrVarToRegister(ArrayAddress* arr) {
    Variable* var = arr->getIndex();

    std::string regWithArrAddress = this->getRegisterWithAddress(arr);
    std::string regWithIndexAddress =  this->getRegisterWithAddress(var);
    std::string regWithSumAddress = this->memory->getFreeRegister();

    this->commands.push_back(new Command(RESET, regWithSumAddress));
    this->commands.push_back(new Command(LOAD, regWithSumAddress + " " + regWithIndexAddress));
    this->commands.push_back(new Command(ADD, regWithSumAddress + " " + regWithArrAddress));
    this->commands.push_back(new Command(LOAD, regWithSumAddress + " " + regWithSumAddress));

    memory->freeRegister(regWithArrAddress, arr->getAddress());
    memory->freeRegister(regWithIndexAddress, var->getAddress());
    return new std::string(regWithSumAddress);
}

void CodeGenerator::checkIfTryModifyIterator(Variable* var) {
    if (auto it = dynamic_cast<Iterator*>(var))
        throw (std::string) "try modify iterator " + var->getName();
}
