#ifndef COMMAND_H
#define COMMAND_H


#include <string>
#include <map>

enum CMD {
    GET,
    PUT,
    LOAD,
    STORE,
    ADD,
    SUB,
    RESET,
    INC,
    DEC,
    SHR,
    SHL,
    JUMP,
    JZERO,
    JODD,
    HALT
};

class Command {
public:
    Command(CMD name);
    Command(CMD name, std::string param);
    void setParam(std::string param);

    std::string parseToString();

private:
    CMD name;
    std::string parameters;
    static std::map<CMD, std::string> CMDstr;
};

#endif //COMMAND_H
