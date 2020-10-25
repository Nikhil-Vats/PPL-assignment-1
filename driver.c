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
    // printTokenStream(s);
    createParseTree(t, s, G);
    // printf("\n%s\n", t->root->child->next->next->next->child->child->child->next->nodeData->nodeName);
    // printParseTree(t->root);
    return 0;
}