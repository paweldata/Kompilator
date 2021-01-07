#ifndef VARIABLE_H
#define VARIABLE_H


#include <string>

class Variable {
public:
    Variable() = default;
    Variable(std::string name, uint address, uint start, uint end);
    uint getAddress();
    void initialize();

    void setToArray();
    uint getAddress(uint arrayIndex);

    void setToArrayWithIndex(Variable* index);
    Variable* getIndex();

private:
    std::string name;
    uint address;
    uint start;
    uint end;
    bool initialized;
    bool isArray;
    bool isArrayWithIndex;
    Variable* index;
};

#endif //VARIABLE_H
