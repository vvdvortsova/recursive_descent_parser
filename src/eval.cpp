#include <cstdlib>
#include <cstdio>
#include "math_tree.h"


int main(int argc, char** argv) {
    if(argc == 2) {
        auto a = doLexer(argv[1]);
        auto tree = syntaxAnalize(&a);
        printf("%f", calculate(tree));
        exit(EXIT_SUCCESS);
    }
    printf("Please!Check your arguments!\n");
    exit(EXIT_FAILURE);
}

