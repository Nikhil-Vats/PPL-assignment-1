#include <stdio.h>
#include <stdbool.h>
#include "parseTree.h"
#include "tokenStream.h"
#include "grammar.h"

// #ifndef STACK_H
// #define STACK_H
typedef struct stackNode {
    char *name;
    bool isTerminal;
    struct stackNode *next;
    struct stackNode *prev;
    parseTreeNode *parentNode;
    bool isChild;
} stackNode;

typedef struct stack {
    int count;
    stackNode *top;
} stack;

void push(stack *st, char *name, parseTreeNode* parent, bool isChild);
void pop(stack *st);
bool isEmpty(stack *st);
bool isTerminal(char* name);
void printStack(stack *st);
grammar* findRules(stack *st, grammar *G);
void createParseTree(parseTree *t, tokenStream *s, grammar *G);
int insertNodesInStack(stack *st, linkedList *rule, parseTreeNode *parent, bool isChild);
void removeNodesFromStack(stack *st, int insertCount);
bool moveForward(linkedList *rule, tokenNode *currToken, stack *st, parseTreeNode *prevNode, grammar *G);
void printChild(parseTreeNode *root, char *parent);
void printSibs(parseTreeNode *root, char *origin);
void printParseTree(parseTree *t);
stack* copyStack(stack *st);
void emptyStack(stack *st);
parseTreeNode* insertNodeInParseTree(stackNode *currSN, tokenNode *currTN, bool isTerminal);
void removeOldNodesFromParseTree(parseTreeNode *ptNode);
void traverseParseTree(parseTree *t, int T);
void printTypeErrors(parseTree *t, int T);
void printTypeExpressionTable(int T);
// #endif