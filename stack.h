#include <stdio.h>
#include <stdbool.h>

#include "parseTree.h"

typedef struct stackNode {
    char *name;
    bool isTerminal;
    struct stackNode *next;
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