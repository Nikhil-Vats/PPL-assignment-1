#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "stack.h"

void push(stack *st, char *name, parseTreeNode* parent, bool isChild) {
    stackNode *temp = (stackNode *)calloc(1, sizeof(stackNode));
    if (!temp) {  
        printf("\nHeap Overflow");  
        exit(1);  
    }

    temp->name = calloc(1, strlen(name)+1);
    strcpy(temp->name, name);
    temp->isTerminal = isTerminal(name);
    temp->parentNode = parent;
    temp->isChild = isChild;
    temp->next = NULL;
    temp->prev = NULL;

    if(st->top == NULL) {
        st->top = temp;
    } else {
        temp->next = st->top;
        st->top->prev = temp;
        st->top = temp;
    }
    st->count++;
}

void emptyStack(stack *st) {
    while(st->top) {
        pop(st);
    }
}

void pop(stack *st) {
    if(st->top == NULL) {
        printf("Stack already empty!");
        exit(1);
    } else if(st->count == 1) {
        stackNode *temp = st->top;
        st->top->parentNode = NULL;
        st->top = NULL;
        free(temp);
        st->count = 0;
    } else {
        stackNode *temp = st->top;
        st->top->parentNode = NULL;
        st->top = st->top->next;
        st->top->prev = NULL;
        st->count--;
        free(temp);
    }
}

bool isEmpty(stack *st) {
    return st->top == NULL;
}

bool isTerminal(char* name) {
    for(int i = 0; i < nonTerminalCount; i++) {
        if(strcmp(name, nonTerminals[i]) == 0) {
            return false;
        }
    }
    return true;
}