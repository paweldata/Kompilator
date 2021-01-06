#ifndef VARIABLE_H
#define VARIABLE_H


#include <string>

class Variable {
public:
    Variable() = default;
    Variable(std::string name, uint address, uint start, uint end);
    uint getAddress();

private:
    std::string name;
    uint address;
    uint start;
    uint end;
};

#endif //VARIABLE_H
