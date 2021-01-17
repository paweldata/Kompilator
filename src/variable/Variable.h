#ifndef VARIABLE_H
#define VARIABLE_H


#include <string>

class Variable {
public:
    Variable() = default;

    Variable(std::string name, uint address) {
        this->name = name;
        this->address = address;
    }

    uint getAddress() {
        if (not initialized)
            throw (std::string) this->name + " not initialized";
        return this->address;
    }

    void initialize() { this->initialized = true; }

    std::string getName() { return this->name; }

protected:
    virtual void functionForDynamicCast() = 0;

    std::string name;
    uint address;
    bool initialized{false};
};

#endif //VARIABLE_H
