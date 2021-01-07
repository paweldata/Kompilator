#include "Command.h"

std::map<CMD, std::string> Command::CMDstr = {
        { GET, "GET" },
        { PUT, "PUT" },
        { LOAD, "LOAD" },
        { STORE, "STORE" },
        { ADD, "ADD" },
        { SUB, "SUB" },
        { RESET, "RESET" },
        { INC, "INC" },
        { DEC, "DEC" },
        { SHR, "SHR" },
        { SHL, "SHL" },
        { JUMP, "JUMP" },
        { JZERO, "JZERO" },
        { JODD, "JODD" },
        { HALT, "HALT" }
};

Command::Command(CMD name) {
    this->name = name;
}

Command::Command(CMD name, std::string param) {
    this->name = name;
    this->parameters = param;
}

void Command::setParam(std::string param) {
    this->parameters = param;
}

std::string Command::parseToString() {
    return Command::CMDstr[this->name] + " " + this->parameters;
}
