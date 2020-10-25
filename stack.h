#include <stdio.h>
#include <stdbool.h>

typedef struct stackNode {
    char *name;
    bool isTerminal;
    struct stackNode *next;
} stackNode;

typedef struct stack {
    int count;
    stackNode *top;
} stack;

void push(stack *st, char *name);
void pop(stack *st);
bool isEmpty(stack *st);
bool isTerminal(char* name);