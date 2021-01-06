#include "Memory.h"

Memory::Memory() {
    this->variables = std::map<std::string, Variable>();
}

Variable Memory::getVariable(std::string name) {
    if (this->variables.find(name) == this->variables.end())
        throw (std::string) name + " not initialized\n";
    return this->variables[name];
}

void Memory::setVariable(std::string name) {
    if (this->variables.find(name) != this->variables.end())
        throw (std::string) name + " already initialized\n";
    this->variables[name] = Variable(name, 0, 0);
}
