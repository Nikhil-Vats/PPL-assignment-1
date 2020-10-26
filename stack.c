#include <stdio.h>
#include <stdbool.h>

#include "stack.h"
#include "grammar.c"

void push(stack *st, char *name, parseTreeNode* parent, bool isChild) {
    stackNode *temp = (stackNode *)malloc(sizeof(stackNode));
    if (!temp) {  
        printf("\nHeap Overflow");  
        exit(1);  
    }

    temp->name = malloc(strlen(name)+1);
    strcpy(temp->name, name);
    temp->isTerminal = isTerminal(name);
    // if(temp->isTerminal) {
    //     printf("%s is a terminal", name);
    // } else {
    //     printf("%s is a non-terminal", name);
    // }
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
        st->top = NULL;
        free(temp);
    } else {
        // printf("\ndeleting\n");
        stackNode *temp = st->top;
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
        // printf("\n%s == %s\n",name,nonTerminals[i]);
        if(strcmp(name, nonTerminals[i]) == 0) {
            return false;
        }
    }
    return true;
}