#include <iostream>
#include "src/math_tree.h"

int main() {
    int size = 0;

    char* expr = getExpressionFromFile("../gr1.txt", &size);
    std::cout << expr << "\n";

    std::vector<Token*> a = doLexer(expr);
    auto tree = syntaxAnalize(&a);
//    gravizDump("../gr1.dot", tree);

//    std::cout << "res = " << calculate(tree);
//    latexDump("../gr1.tex", tree);

    return 0;
}
