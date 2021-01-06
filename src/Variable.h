#ifndef VARIABLE_H
#define VARIABLE_H


#include <string>

class Variable {
public:
    Variable() = default;
    Variable(std::string name, uint address, uint first);
    uint getAddress();

private:
    std::string name;
    uint address;
    uint first;
};

#endif //VARIABLE_H
