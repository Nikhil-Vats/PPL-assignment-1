#include <stdio.h>
#include <stdbool.h>

#ifndef STACK_H
#define STACK_H

typedef struct parseTreeNode parseTreeNode;

typedef struct linkedList linkedList;
typedef struct grammar grammar;

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

int insertNodesInStack(stack *st, linkedList *rule, parseTreeNode *parent, bool isChild);
void removeNodesFromStack(stack *st, int insertCount);

stack* copyStack(stack *st);
void emptyStack(stack *st);

extern char* nonTerminals[80];
extern int nonTerminalCount;

#endif