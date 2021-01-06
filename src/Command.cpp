#include "Command.h"

Command::Command(std::string name) {
    this->name = name;
}

void Command::setParam(std::string param) {
    this->parameters = param;
}
