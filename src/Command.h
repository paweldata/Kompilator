#ifndef COMMAND_H
#define COMMAND_H


#include <string>

class Command {
public:
    Command(std::string name);
    void setParam(std::string param);

private:
    std::string name;
    std::string parameters;
};

#endif //COMMAND_H
