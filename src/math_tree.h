/**
* @file         math_tree.h
* @author       Dvortsova Varvara BSE182 HSE
*/
#ifndef PARSE_TREE_OF_MATHEMATICAL_EXPRESSIONS_MATH_TREE_H
#define PARSE_TREE_OF_MATHEMATICAL_EXPRESSIONS_MATH_TREE_H

#include <vector>
#include <fstream>

enum OP_TYPE {
    ADD = 0,
    SUB = 1,
    DIV = 2,
    MUL = 3,
    POW = 4,
    X   = 5,
    Y   = 6,
    SIN = 7,
    COS = 8,
    VAR = 10,
    NUM = 9
};


enum OP_PAIR {
    OPEN,
    CLOSE
};

/**
 * struct is describing of the node of the ast tree
 */
typedef struct Node {
    OP_TYPE type;
    char chVar;
    double value;
    double index;
    Node* leftChild;
    Node* rightChild;
}Node;

/**
 * Super class for all tokens
 */
class Token {
    int num{};
    int priority = 0;
public:
    explicit Token(int _priority):priority(_priority){};
    virtual ~Token()= default;
    void setNumber(int val){ this->num = val;}
    int getNumber() const { return num;}
    int getPriority() const { return priority;};
};

class Num: public Token {
    double value;
public:
    explicit Num(double val): Token(0), value(val) {}
    double getValue() const { return value;}
};

class Var:     public Token {
    char ch;
public:
    explicit Var(char _ch): Token(0), ch(_ch){}
    char getCh() const { return ch;}
};

class UnMinus: public Token {
public:
    explicit UnMinus(): Token(2){};
};
class Sin:     public Token {
public:
    explicit Sin(): Token(3){};
};
class Cos:     public Token {
public:
    explicit Cos(): Token(3){};
};
class Mul:     public Token {
public:
    explicit Mul(): Token(2){};
};
class Add:     public Token {
public:
    explicit Add(): Token(1){};
};
class Sub:     public Token {
public:
    explicit Sub(): Token(1){};
};
class Div:     public Token {
public:
    explicit Div(): Token(2){}
};

class Pow:     public Token {
public:
    explicit Pow(): Token(3){}
};

class Pair:    public Token {
    OP_PAIR pType;
public:
    Pair(OP_PAIR _type): Token(0), pType(_type) {}
    OP_PAIR getType() const { return pType;}
};

class End:     public Token {
public:
    explicit  End(): Token(0){}
};


char* getExpressionFromFile(char* fName, int* size);


/**
 * I use recursive descent algo
 * @param tokens
 * @return
 */
Node* syntaxAnalize(std::vector<Token*>* tokens);

/**
 * Get tokens from exps
 * @param expr
 * @return
 */
std::vector<Token*> doLexer(char* expr);

/**
 * Dumps tree into graviz format
 * @param outPath
 * @param tree
 */
void gravizDump(char* outPath, Node* tree);

/**
 * Helping method to walk the tree when gravizDump is doing
 * @param myfile
 * @param tree
 * @param index
 */
void gravizDeepWriting(std::ofstream& myfile, Node* tree, int* index);

/**
 * Dumps tree into latex format
 * @param outPath
 * @param tree
 */
void latexDump(char* outPath, Node* tree);

/**
 * Helping method to walk the tree when latexDump is doing
 * @param myfile
 * @param root
 */
void latexDeepWriting(std::ofstream& myfile, Node* root);

const char* getNameOfOp(OP_TYPE type);

/**
 * Calculate the value of expression using tree
 * @param tree
 * @return
 */
double calculate(Node* tree);

Node *getG(std::vector<Token *>::iterator* iterator);

Node *getE(std::vector<Token *>::iterator* iterator);

Node *getT(std::vector<Token *>::iterator* iterator);

Node *createNode(Node *pNode, Node *pNode1, OP_TYPE type, int index);

Node *getP(std::vector<Token *>::iterator* iterator);

Node *getN(std::vector<Token *>::iterator* iterator);

bool requirePair(std::vector<Token *>::iterator* iterator);

Node *getS(std::vector<Token *>::iterator *pIterator);

Node *getId(std::vector<Token *>::iterator *pIterator);

Node *createUnaryNode(Node *pNode, OP_TYPE type, int number);

#endif //PARSE_TREE_OF_MATHEMATICAL_EXPRESSIONS_MATH_TREE_H
