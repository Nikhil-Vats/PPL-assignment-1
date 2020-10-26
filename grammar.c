#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "grammar.h"

char* nonTerminals[RULES_LENGTH];
int nonTerminalCount = 0;

void addNode(linkedList *l, char* nodeName) {
    //create a link
    node *link = (node *)malloc(sizeof(node));

    link->name = malloc(strlen(nodeName) + 1);
    strcpy(link->name, nodeName);
    link->next = NULL;

    if (l->first == NULL && l->last == NULL) {
        l->first = l->last = link;
        if(nonTerminals[nonTerminalCount] != nodeName) {
            nonTerminals[nonTerminalCount] = malloc(strlen(nodeName)+1);
            strcpy(nonTerminals[nonTerminalCount++], nodeName);
        }
    } else {
        l->last->next = link;
        l->last = link;
    }
    l->count++;
    return;
}

void printGrammar(grammar *G) {
    for(int i = 0; i < G->rulesCount; i++) {
        linkedList *ll = G->arr[i];
        int nc = 0; 
        node *temp = ll->first;
        printf("%s -->  ", temp->name);
        temp = temp->next;
        while(temp != NULL) {
            printf("%s  ", temp->name);
            temp = temp->next;
            nc++;
        }
        printf("\n");
    }
}

void readGrammar(char* filename, grammar *G) {
    FILE* file = fopen(filename, "r");
    if(file == NULL) {
        printf("Error opening file!");
        exit(1);
    }
    char line[256];
    int line_no = 0;
    while (fgets(line, sizeof(line), file)) {
        G->arr[line_no] = (linkedList *)malloc(sizeof(linkedList));
        G->arr[line_no]->first = NULL;
        G->arr[line_no]->last = NULL;
        G->arr[line_no]->count = 0;

        char *token;
        token = strtok(line, " \n");
        while (token != NULL)
        {
            addNode(G->arr[line_no], token);
            token = strtok(NULL, " \n");
        }
        line_no++;
    }
    G->rulesCount = line_no;

    // for(int i = 0; i < line_no; i++) {
    //     linkedList *ll = G->arr[i];
    //     int nc = 0; 
    //     node *temp = ll->first;
    //     printf("%s -->  ", temp->name);
    //     temp = temp->next;
    //     while(temp != NULL) {
    //         printf("%s  ", temp->name);
    //         temp = temp->next;
    //         nc++;
    //     }
    //     printf("\n");
    // }
    fclose(file);
}