#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"

int main() {
    grammar *G = (grammar *)malloc(sizeof(grammar));
    tokenStream *s = (tokenStream*)malloc(sizeof(tokenStream));
    parseTree *t = (parseTree *)malloc(sizeof(parseTree));
    readGrammar("grammar.txt",G);
    tokeniseSourcecode("test.txt",s);
    int option;
    printf("\nPlease enter a number between 0 and 4 and press ENTER -\n");
    printf("0 -> EXIT\n");
    printf("1 -> Creating the parse tree\n");
    printf("2 -> Traversing the parse tree and printing type errors\n");
    printf("3 -> Printing the parse tree\n");
    printf("4 -> Printing the type expression table\n");
    scanf("%d", &option);

    if(option != 0) {
        do {
            if(option == 1) {
                createParseTree(t, s, G);
            } else if(option == 2) {
                traverseParseTree(t, option);
                printTypeErrors(t, option);
            } else if(option == 3) {
                printParseTree(t);
            } else if(option == 4) {
                printTypeExpressionTable(option);
            }
            printf("\nPlease enter a number between 0 and 4 -\n");
            printf("0 -> EXIT\n");
            printf("1 -> Creating the parse tree\n");
            printf("2 -> Traversing the parse tree and printing type errors\n");
            printf("3 -> Printing the parse tree\n");
            printf("4 -> Printing the type expression table\n");
            scanf("%d", &option);
        } while(option != 0);
    }
    return 0;
}