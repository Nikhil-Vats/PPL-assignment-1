#include <stdio.h>
#include <stdbool.h>

#ifndef GRAMMAR_H
#define GRAMMAR_H

#define RULES_LENGTH 64

typedef struct node {
    char *name;
    struct node *next;
} node;

typedef struct linkedList {
	int count;
	struct node *first;
    struct node *last;
} linkedList;

typedef struct grammar {
    int rulesCount;
    linkedList *arr[RULES_LENGTH];
} grammar;

void addNode(linkedList *head, char* ele);

void readGrammar(char* filename, grammar *G);

void printGrammar(grammar *G);

extern char* nonTerminals[RULES_LENGTH];
extern int nonTerminalCount;

#endif