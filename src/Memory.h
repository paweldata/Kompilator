#ifndef MEMORY_H
#define MEMORY_H


#include <map>

#include "Variable.h"

class Memory {
public:
    Memory();
    Variable getVariable(std::string name);
    void setVariable(std::string name);

private:
    std::map<std::string, Variable> variables;
};

#endif //MEMORY_H
