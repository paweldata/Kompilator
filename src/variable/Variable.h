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
    virtual void functionForDynamicCast() = 0;

    std::string name;
    uint address;
    bool initialized;
};

#endif //VARIABLE_H
