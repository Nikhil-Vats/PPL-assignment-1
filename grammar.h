#include <stdio.h>
#define MAX_LENGTH 100
#define RULES_LENGTH 100

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
    linkedList *arr[RULES_LENGTH];
} grammar;

void addNode(linkedList *head, char* ele);

void readGrammar(char* filename, grammar G);

// struct node *deleteNode(struct linkedList* head);

// void printList(struct linkedList * head);

// int search(struct linkedList* head, int ele);

// void node* delete(struct linkedList* head, int ele);