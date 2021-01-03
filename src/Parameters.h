#ifndef PARAMETERS_H
#define PARAMETERS_H


#include <stdio.h>

class Parameters {
public:
    Parameters(int argc, char** argv) {
        if (argc < 2)
            return;
        this->input = fopen(argv[1], "r");
        this->output = fopen(argv[2], "w");
    }

    void checkCorrectness() {
        if (!this->areValid()) {
            this->showError();
            exit(-1);
        }
    }

    FILE* getInput() { return this->input; }
    FILE* getOutput() {return this->output; }

private:
    bool areValid() const { 
        return this->input != NULL && this->output != NULL;
    }

    static void showError() {
        printf("Give parameters:\n"
               "./compiler <input_filename> <output_filename>\n");
    }

    FILE* input;
    FILE* output;
};


#endif //PARAMETERS_H
