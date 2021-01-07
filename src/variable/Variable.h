#ifndef VARIABLE_H
#define VARIABLE_H


#include <string>

class Variable {
public:
    Variable() = default;
    Variable(std::string name, uint address);

    uint getAddress();
    void initialize();
    std::string getName();

protected:
    std::string name;
    uint address;
    bool initialized;
};

#endif //VARIABLE_H
