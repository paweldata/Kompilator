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
    if (auto arrAddress = dynamic_cast<ArrayAddress*>(var)) {
        this->writeArrayAddress(arrAddress);
        return;
    }
    if (auto constant = dynamic_cast<Constant*>(var)) {
        this->writeConstant(constant);
        return;
    }
    std::string reg = this->getRegisterWithAddress(var);
    this->commands.push_back(new Command(PUT, reg));
    memory->freeRegister(reg, var->getAddress());
}

void CodeGenerator::assignValue(Variable* var, std::string reg) {
    if (ArrayAddress* arrAddress = dynamic_cast<ArrayAddress*>(var)) {
        this->assignArrayValue(arrAddress, reg);
        return;
    }

    this->checkIfTryModifyIterator(var);
    this->assignValueAfterChecks(var, reg);
}

std::string* CodeGenerator::setVarToRegister(Variable* var) {
    if (ArrayAddress* arrAddress = dynamic_cast<ArrayAddress*>(var))
        return this->setArrVarToRegister(arrAddress);
    if (auto constant = dynamic_cast<Constant*>(var))
        return new std::string(this->getRegisterWithValue(constant->getValue()));

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

void CodeGenerator::assignValueAfterChecks(Variable* var, std::string reg) {
    var->initialize();
    std::string varReg = this->getRegisterWithAddress(var);
    this->commands.push_back(new Command(STORE, reg + " " + varReg));

    memory->freeRegister(reg, -1);
    memory->freeRegister(varReg, var->getAddress());
}

void CodeGenerator::writeConstant(Constant* constant) {
    std::string regValue = this->getRegisterWithValue(constant->getValue());
    std::string regAddress = this->getRegisterWithValue(0);

    this->commands.push_back(new Command(STORE, regValue + " " + regAddress));
    this->commands.push_back(new Command(PUT, regAddress));

    memory->freeRegister(regValue, constant->getValue());
    memory->freeRegister(regAddress, 0);
}

std::string CodeGenerator::getRegisterWithAddress(Variable* var) {
    std::string reg;
    bool isAlreadySet;
    std::tie(reg, isAlreadySet) = this->memory->getFreeRegister(var);
    if (not isAlreadySet)
        this->setRegisterValue(reg, var->getAddress());
    return reg;
}

std::string CodeGenerator::getRegisterWithAddress(ArrayAddress* arr) {
    Variable* var = arr->getIndex();

    std::string* reg1 = this->setVarToRegister(var);
    std::string reg2 = this->getRegisterWithValue(arr->getArrAddress());

    this->commands.push_back(new Command(ADD, reg2 + " " + *reg1));
    this->memory->freeRegister(*reg1, -1);
    *reg1 = this->getRegisterWithValue(arr->getElemAddress());
    this->commands.push_back(new Command(SUB, reg2 + " " + *reg1));

    this->memory->freeRegister(*reg1, arr->getElemAddress());
    return reg2;
}

std::string CodeGenerator::getRegisterWithValue(uint64_t value) {
    std::string reg;
    bool isAlreadySet;
    std::tie(reg, isAlreadySet) = this->memory->getFreeRegister(value);
    if (not isAlreadySet)
        this->setRegisterValue(reg, value);
    return reg;
}

void CodeGenerator::setRegisterValue(std::string reg, uint64_t value) {
    this->commands.push_back(new Command(RESET, reg));
    std::string binary = this->decToBin(value);

    if (binary == "")
        return;

    this->commands.push_back(new Command(INC, reg));
    binary.erase(0, 1);

    for (char c : binary) {
        this->commands.push_back(new Command(SHL, reg));
        if (c == '1')
            this->commands.push_back(new Command(INC, reg));
    }
}

std::string CodeGenerator::decToBin(uint64_t value) {
    std::string binary;
    while (value > 0) {
        binary = (value % 2 == 0 ? "0" : "1") + binary;
        value /= 2;
    }
    return binary;
}

void CodeGenerator::readArrayAddress(ArrayAddress* arr) {
    std::string reg = this->getRegisterWithAddress(arr);
    this->commands.push_back(new Command(GET, reg));
    memory->freeRegister(reg, -1);
}

void CodeGenerator::writeArrayAddress(ArrayAddress* arr) {
    std::string reg = this->getRegisterWithAddress(arr);
    this->commands.push_back(new Command(PUT, reg));
    memory->freeRegister(reg, -1);
}

void CodeGenerator::assignArrayValue(ArrayAddress* arr, std::string reg) {
    std::string reg2 = this->getRegisterWithAddress(arr);
    this->commands.push_back(new Command(STORE, reg + " " + reg2));
    memory->freeRegister(reg, -1);
    memory->freeRegister(reg2, -1);
}

std::string* CodeGenerator::setArrVarToRegister(ArrayAddress* arr) {
    std::string reg = this->getRegisterWithAddress(arr);
    this->commands.push_back(new Command(LOAD, reg + " " + reg));
    return new std::string(reg);
}

void CodeGenerator::checkIfTryModifyIterator(Variable* var) {
    if (auto it = dynamic_cast<Iterator*>(var))
        throw (std::string) "try modify iterator " + var->getName();
}
