#ifndef CODEGENERATOR_H
#define CODEGENERATOR_H


#include <vector>
#include <stack>

#include "Command.h"
#include "Memory.h"
#include "variable/ArrayAddress.h"

struct Condition {
    uint beforeCondPtr;
    uint afterCondPtr;
    Command* falseJump;
};

struct IfElseParam {
    uint ptr;
    Command* jump;
};

class CodeGenerator {
public:
    CodeGenerator(Memory* memory);

    void readVariable(Variable* var);
    void writeVariable(Variable* var);

    void setConstValue(Variable* var);

    void assignValue(Variable* var, std::string reg);
    std::string* setVarToRegister(Variable* var);

    void endGenerateCode();
    std::string getCode();

    class Operations {
    public:
        Operations(CodeGenerator& code) : codeGen(code) {};
        std::string* add(Variable* var1, Variable* var2);
        std::string* sub(Variable* var1, Variable* var2);
        std::string* mul(Variable* var1, Variable* var2);
        std::string* div(Variable* var1, Variable* var2);
        std::string* mod(Variable* var1, Variable* var2);

    private:
        CodeGenerator& codeGen;
    };

    class Conditions {
    public:
        Conditions(CodeGenerator& code) : codeGen(code) {};
        Condition equal(Variable* var1, Variable* var2);

    private:
        CodeGenerator& codeGen;
    };

    class FlowControler {
    public:
        FlowControler(CodeGenerator& code) : codeGen(code) {};

        void singleIf(Condition cond);
        IfElseParam ifElseFirst(Condition cond);
        void ifElseSecond(IfElseParam param);

        void whileLoop(Condition cond);
        uint repeatUntilFirst();
        void repeatUntilSecond(Condition cond, uint ptr);

    private:
        CodeGenerator& codeGen;
    };

    Operations* doOperation() { return &this->operations; }
    Conditions* makeCondition() { return &this->conditions; }
    FlowControler* flowControl() { return &this->flowControler; }

private:
    void setRegisterValue(std::string reg, uint value);
    std::string decToBin(uint value);

    void readArrayAddress(ArrayAddress* arr);
    void writeArrayAddress(ArrayAddress* arr);
    void assignArrayValue(ArrayAddress* arr, std::string reg);
    std::string* setArrVarToRegister(ArrayAddress* arr);

    Memory* memory;
    Operations operations;
    Conditions conditions;
    FlowControler flowControler;
    std::vector<Command*> commands;
    std::stack<Command*> stack;
};

#endif //CODEGENERATOR_H
