#ifndef CODEWRITER_H
#define CODEWRITER_H


#include <stdio.h>

class CodeWriter {
public:
    CodeWriter();
    void writeToFile(FILE* output);
};

#endif //CODEWRITER_H
