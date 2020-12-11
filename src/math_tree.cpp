/**
* @file         math_tree.cpp
* @brief        Definitions of methods for recursive descent parser
* @author       Dvortsova Varvara BSE182 HSE
*/
#include "math_tree.h"
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stack>
#include <cmath>
#include <iostream>
#define  EQUATION_EPS 1e-9

/**
* @brief      Compare a double variable with zero
* @param[in]  a  double variable
* @return     0 if a == 0 else 1
*/
static bool isZero(double a) {
    return fabs(a) < EQUATION_EPS;
}

const char* getNameOfOp(OP_TYPE type) {
    switch(type) {
        case ADD:
            return "+";
        case SUB:
            return "-";
        case DIV:
            return "/";
        case MUL:
            return "*";
        case POW:
            return "^";
        case X:
            return "x";
        case Y:
            return "y";
        case SIN:
            return "sin";
        case COS:
            return "cos";
        case NUM:
            return nullptr;
    }
    return nullptr;
}

char* getExpressionFromFile(char* fName, int* size) {
    assert(fName != NULL);

    FILE* file = NULL;
    file = fopen(fName, "r");
    if(!file) {
        fprintf(stderr,"Do not open = %s\n", fName);
        exit(EXIT_FAILURE);
    }

    // define size of file
    fseek(file, 0, SEEK_END);
    *size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* arr = static_cast<char *>(calloc(*size, *size * sizeof(char)));
    if (arr) {
        int res = fread(arr, 1, *size, file);
        if(res != *size) {
            fprintf(stderr,"Do not read file to buffer = %s\n", fName);
            exit(EXIT_FAILURE);
        }
    }
    fclose(file);
    return arr;
}

bool getDoubleNumber(char* mnemonicStart, double* number, char** endptr) {
    char* endPtr = NULL;
    //cast string to double
    *number = strtod(mnemonicStart, &endPtr);
    *endptr = endPtr;
    return endPtr != mnemonicStart;
}

/**
 * Simple Lexer
 * @param expr
 * @return tokens
 */
std::vector<Token*> doLexer(char* expr) {
    assert(expr != nullptr);
    int countOP = 0;
    std::vector<Token*> tokens;
    char* token = strtok(expr, " ");
    while( token != nullptr ) {
        printf( " %s\n", token );
        for (; token != nullptr && ((strncmp(token,"",1) != 0) && (strncmp(token,"\n",1) != 0)); ) {
            if(strncmp(token,"(",1) == 0) {
                auto elem = new Pair(OPEN);
                elem->setNumber(countOP);
                tokens.push_back(elem);
                countOP++;
                if(strlen(token) > 1)token++;
                else break;
            }
            else if(strncmp(token,"+",1) == 0) {
                auto elem = new Add();
                elem->setNumber(countOP);
                tokens.push_back(elem);
                countOP++;
                if(strlen(token) > 1)token++;
                else break;
            }
            else if(strncmp(token,"-",1) == 0) {
                auto elem = new Sub();
                elem->setNumber(countOP);
                tokens.push_back(elem);
                countOP++;
                if(strlen(token) > 1)token++;
                else break;
            }
            else if(strncmp(token,"/",1) == 0) {
                auto elem = new Div();
                elem->setNumber(countOP);
                tokens.push_back(elem);
                countOP++;
                ++token;
            }
            else if(strncmp(token,"*",1) == 0) {
                auto elem = new Mul();
                elem->setNumber(countOP);
                tokens.push_back(elem);
                countOP++;
                if(strlen(token) > 1)token++;
                else break;
            }
            else if(strncmp(token,"^",1) == 0) {
                auto elem = new Pow();
                elem->setNumber(countOP);
                tokens.push_back(elem);
                countOP++;
                if(strlen(token) > 1)token++;
                else break;
            }
            else if(strncmp(token,"x",1) == 0 || strncmp(token,"y",1) == 0) {
                auto elem = new Var(token[0]);
                elem->setNumber(countOP);
                tokens.push_back(elem);
                countOP++;
                if(strlen(token) > 1)token++;
                else break;
            }
            else if(strncmp(token,"sin", 3) == 0) {
                auto elem = new Sin();
                elem->setNumber(countOP);
                tokens.push_back(elem);
                countOP++;
                if(strlen(token) > 1)token += 3;
                else break;
            }
            else if(strncmp(token,"cos", 3) == 0) {
                auto elem = new Cos();
                elem->setNumber(countOP);
                tokens.push_back(elem);
                countOP++;
                if(strlen(token) > 1)token += 3;
                else break;
            }
            else if(strncmp(token,")", 1) == 0) {
                auto elem = new Pair(CLOSE);
                elem->setNumber(countOP);
                tokens.push_back(elem);
                countOP++;
                if(strlen(token) > 1)token++;
                else break;
            }
            else {
                double num = 0;
                char* next = nullptr;
                if(getDoubleNumber(token, &num, &next)) {
                    if(num < 0) {
                        auto elem = new UnMinus();
                        elem->setNumber(countOP);
                        tokens.push_back(elem);
                        num = -num;
                        countOP++;
                    }
                    auto elem = new Num(num);
                    elem->setNumber(countOP++);
                    tokens.push_back(elem);
                    if((strncmp(next,"",1) != 0) && (strncmp(next,"\n",1) != 0)) token = next;
                    else token = nullptr;
                } else {
                    fprintf(stderr,"Syntax Error: Unexpected sign: %s!\n", token);
                    exit(EXIT_FAILURE);
                }
            }
        }
        token = strtok(nullptr, " ");
    }

    //for debug
//    std::vector<Token*>::iterator it;
//    for (it = std::begin(tokens); it != std::end(tokens) ; ++it) {
//        if(dynamic_cast<UnMinus*>(*it) != nullptr) {
//            std::cout << "unMinus\n";
//        } else if (dynamic_cast<Pair*>(*it) != nullptr) {
//            std::cout << "Pair(" << dynamic_cast<Pair*>(*it)->getType() << ")\n";
//        } else if (dynamic_cast<Add*>(*it) != nullptr) {
//            std::cout << "add\n";
//        } else if (dynamic_cast<Sub*>(*it) != nullptr) {
//            std::cout << "sub\n";
//        } else if (dynamic_cast<Mul*>(*it) != nullptr) {
//            std::cout << "mul\n";
//        } else if (dynamic_cast<Div*>(*it) != nullptr) {
//            std::cout << "div\n";
//        } else if (dynamic_cast<Pow*>(*it) != nullptr) {
//            std::cout << "pow\n";
//        } else if (dynamic_cast<Var*>(*it) != nullptr) {
//            std::cout << "Var(" << dynamic_cast<Var*>(*it)->getCh() << ")\n";
//        } else if (dynamic_cast<Sin*>(*it) != nullptr) {
//            std::cout << "sin\n";
//        } else if (dynamic_cast<Cos*>(*it) != nullptr) {
//            std::cout << "cos\n";
//        }
//        else if (dynamic_cast<Num*>(*it) != nullptr) {
//            std::cout << "Num(" << dynamic_cast<Num*>(*it)->getValue() << ")\n";
//        }
//    }

    tokens.push_back(new End);
    return tokens;
}

Node* syntaxAnalize(std::vector<Token*>* tokens) {
    auto iter = tokens->begin();
    Node* tree = getG(&iter);
    return tree;
}

Node* getG(std::vector<Token*>::iterator* iter) {
    Node* val = getE(iter);
    if(dynamic_cast<End*>(**iter) != nullptr) {
        return val;
    } else {
        //TODO syntx error
        return nullptr;
    }
}

Node* getE(std::vector<Token*>::iterator* iter) {
    Node* lval = getT(iter);
    while(dynamic_cast<Add*>(**iter) != nullptr || dynamic_cast<Sub*>(**iter) != nullptr) {
        Token* op = **iter;
        (*iter)++;
        Node* rval = getT(iter);
        if(dynamic_cast<Add*>(op) != nullptr) {
            lval = createNode(lval, rval, ADD,(**iter)->getNumber());
        } else lval = createNode(lval, rval, SUB, (**iter)->getNumber());
    }
    return lval;
}

Node* createNode(Node* lNode, Node* rNode, OP_TYPE type, int index) {
    Node* node =  new Node();
    node->type = type;
    node->value = 0;
    node->index = index;
    node->leftChild = lNode;
    node->rightChild = rNode;
    return node;
}

Node* getT(std::vector<Token*>::iterator* iter) {
    Node* lval = getS(iter);
    while (dynamic_cast<Mul*>(**iter) != nullptr || dynamic_cast<Div*>(**iter) != nullptr) {
        Token* op = **iter;
        (*iter)++;
        Node* rval = getS(iter);
        if(dynamic_cast<Mul*>(op) != nullptr) {
            lval = createNode(lval, rval, MUL, (**iter)->getNumber());
        } else
            lval = createNode(lval, rval, DIV, (**iter)->getNumber());
    }
    return lval;
}

Node* getS(std::vector<Token*>::iterator *iter) {
    Node* lval = getP(iter);
    while (dynamic_cast<Pow*>(**iter) != nullptr) {
        Token* op = **iter;
        (*iter)++;
        Node* rval = getP(iter);
        if(dynamic_cast<Pow*>(op) != nullptr) {
            lval = createNode(lval, rval, POW, (**iter)->getNumber());
        } else {
            //todo
            return nullptr;
        }
    }
    return lval;
}


Node* getP(std::vector<Token*>::iterator* iter) {
    if (dynamic_cast<Pair*>(**iter) != nullptr) {
        auto item = dynamic_cast<Pair*>(**iter);
        if(item->getType() == OPEN) {
            (*iter)++;
            Node* lval = getE(iter);
            if(requirePair(iter)) {
                return lval;
            } else {
                //add message or exception and so on
                return nullptr;
            }
        }
    } else if(dynamic_cast<Num*>(**iter) != nullptr) {
        return getN(iter);
    } else {
        return getId(iter);
    }
    return nullptr;
}

Node* getId(std::vector<Token*>::iterator* iter) {
    if(dynamic_cast<Var*>(**iter) != nullptr) {
        Node* nodeVar =  new Node();
        nodeVar->type = VAR;
        auto elem = dynamic_cast<Var*>(**iter);
        nodeVar->chVar = elem->getCh();
        nodeVar->leftChild = nullptr;
        nodeVar->rightChild = nullptr;
        nodeVar->index = elem->getNumber();
        (*iter)++;
        return nodeVar;
    } else if (dynamic_cast<Cos*>(**iter) != nullptr || dynamic_cast<Sin*>(**iter) != nullptr) {
        Token* op = **iter;
        (*iter)++;
        if (dynamic_cast<Pair*>(**iter) != nullptr) {
            auto item = dynamic_cast<Pair*>(**iter);
            if(item->getType() == OPEN) {
                (*iter)++;
                Node* rval = getE(iter);
                if(requirePair(iter)) {
                    if(dynamic_cast<Cos*>(op) != nullptr) {
                        return createUnaryNode(rval, COS, (**iter)->getNumber());
                    } else {
                        return createUnaryNode(rval, SIN, (**iter)->getNumber());
                    }
                } else {
                    //add message or exception and so on
                    return nullptr;
                }
            }
        }
    }
    return nullptr;
}

Node* createUnaryNode(Node* val, OP_TYPE type, int number) {
    Node* nodeVar =  new Node();
    nodeVar->type = type;
    nodeVar->leftChild = nullptr;
    nodeVar->rightChild = val;
    nodeVar->index = number;
    return nodeVar;
}

bool requirePair(std::vector<Token*>::iterator* iter) {
    if (dynamic_cast<Pair*>(**iter) != nullptr) {
        auto item = dynamic_cast<Pair*>(**iter);
        if(item->getType() == CLOSE) {
            (*iter)++;
            return  true;
        }
    }
    return false;
}

Node* getN(std::vector<Token*>::iterator* iter) {
    if(dynamic_cast<Num*>(**iter) != nullptr) {
        Node* nodeNum = new Node();
        nodeNum->type = NUM;
        auto elem = dynamic_cast<Num*>(**iter);
        nodeNum->value =  elem->getValue();
        nodeNum->leftChild = nullptr;
        nodeNum->rightChild = nullptr;
        nodeNum->index = elem->getNumber();
        (*iter)++;
        return nodeNum;
    }
    //ToDO add exception or so on
    return nullptr;
}



void gravizDump(char* outPath, Node* tree) {
    assert(outPath);
    assert(tree);
    int index = 0;
    std::ofstream myfile;
    myfile.open (outPath);
    myfile << "graph astdump {\n";
    myfile << "res [shape=doubleoctagon]" << "[label=\"result = "<< calculate(tree)<<"\"]\n";
    gravizDeepWriting(myfile, tree, &index);
    myfile << "}\n";

    myfile.close();

}
double calculate(Node* tree) {
    if(tree == nullptr)
        return 0;
    if(tree->type == COS) {
        double b = calculate(tree->rightChild);
        return cos(b);
    } else if(tree->type == SIN) {
        double b = calculate(tree->rightChild);
        return sin(b);
    }
    if(tree->type != NUM) {
        double a = calculate(tree->leftChild);
        double b = calculate(tree->rightChild);
        switch (tree->type) {
            case ADD:
                return a + b;
            case MUL:
                return a * b;
            case SUB:
                return a - b;
            case DIV:
                return a / b;
            case POW:
                return pow(a,b);
            case NUM:
                break;
        }
    }
    return tree->value;
}



void gravizDeepWriting(std::ofstream& myfile, Node* tree, int *index) {
    if(tree == nullptr || tree->type == NUM || tree->type == VAR)
        return;
    if (tree->type == COS || tree->type == SIN) {
        myfile << "_" << tree->index << "_" << tree->type << "[shape=box, color=red,label=\"" << getNameOfOp(tree->type) << "\"]\n";

        myfile << "_" << tree->index << "_" << tree->type << " -- ";
        if(tree->rightChild->type == NUM) {
            myfile << "_" << tree ->rightChild->index << "N" << tree ->rightChild->value << ";\n";
            myfile << "_" << tree ->rightChild->index << "N" << tree ->rightChild->value << " [label=\"" << tree ->rightChild->value << "\"]\n";
        }
        else if(tree->rightChild->type == VAR) {
            myfile << "_" << tree ->rightChild->index << "V" << tree ->rightChild->value << ";\n";
            myfile << "_" << tree ->rightChild->index << "V" << tree ->rightChild->value << " [label=\"" << tree ->rightChild->chVar << "\"]\n";
        } else
            myfile << "_" << tree->rightChild->index << "_" << tree->rightChild->type << ";\n";
        gravizDeepWriting(myfile, tree->leftChild, index);
        gravizDeepWriting(myfile, tree->rightChild, index);
    } else {
        myfile << "_" << tree->index << "_" << tree->type << "[shape=box, color=blue,label=\"" << getNameOfOp(tree->type) << "\"]\n";
        myfile << "_" << tree->index << "_" << tree->type << " -- ";
        if(tree->leftChild->type == NUM) {
            myfile << "_" << tree ->leftChild->index << "N" ;
            char temp[100] ="";
            if(isZero(tree->leftChild->value)) {
                sprintf(temp,"_%d0",(*index)++);
            }
            myfile << temp << "\n";
            myfile << "_" << tree->leftChild->index << "N" << temp << " [label=\"" << tree->leftChild->value << "\"]\n";

        } else if (tree->leftChild->type == VAR) {
            myfile << "_" << tree ->leftChild->index << "V" ;
            myfile << "_" << tree->leftChild->index << "V" << " [label=\"" << tree->leftChild->chVar << "\"]\n";

        } else {
            myfile << "_" << tree->leftChild->index << "_" << tree->leftChild->type << ";\n";
        }

        myfile << "_" << tree->index << "_" << tree->type << " -- ";
        if(tree->rightChild->type == NUM) {
            myfile << "_" << tree ->rightChild->index << "N" << tree ->rightChild->value << ";\n";
            myfile << "_" << tree ->rightChild->index << "N" << tree ->rightChild->value << " [label=\"" << tree ->rightChild->value << "\"]\n";
        }
        else if(tree->rightChild->type == VAR) {
            myfile << "_" << tree ->rightChild->index << "V" << tree ->rightChild->value << ";\n";
            myfile << "_" << tree ->rightChild->index << "V" << tree ->rightChild->value << " [label=\"" << tree ->rightChild->chVar << "\"]\n";
        } else
            myfile << "_" << tree->rightChild->index << "_" << tree->rightChild->type << ";\n";

        gravizDeepWriting(myfile, tree->leftChild, index);
        gravizDeepWriting(myfile, tree->rightChild, index);
    }
}




void latexDump(char* outPath, Node* tree) {
    assert(outPath);
    assert(tree);
    std::ofstream myfile;
    myfile.open (outPath);
    myfile << "\\documentclass{article}\n";
    myfile << "\\begin{document}\n";
    myfile << "$$";
    latexDeepWriting(myfile, tree);
    myfile << "$$";
    myfile << "\n";
    myfile << "\\end{document}\n";
    myfile.close();
}

void latexDeepWriting(std::ofstream& myfile, Node* root) {
    if(root == nullptr)
        return;
    if(root->type != NUM) {
        if (root->type == DIV) {
            myfile << "\\frac{";
            latexDeepWriting(myfile, root->leftChild);
            myfile << "}{";
            latexDeepWriting(myfile, root->rightChild);
            myfile << "}";
            return;
        }
        myfile << "(";
        latexDeepWriting(myfile, root->leftChild);
        myfile << getNameOfOp(root->type);
        latexDeepWriting(myfile, root->rightChild);
        myfile << ")";
    } else {
        myfile << " "<< root->value << " ";
    }
}


