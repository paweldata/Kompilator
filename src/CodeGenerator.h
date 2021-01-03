#ifndef CODEGENERATOR_H
#define CODEGENERATOR_H


#include <stdio.h>

class CodeGenerator {
public:
    CodeGenerator(FILE* output) {
        this->output = output;
    }

private:
    FILE* output;
};

#endif //CODEGENERATOR_H