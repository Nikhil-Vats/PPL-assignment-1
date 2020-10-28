#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parseTree.h"

int main() {
    grammar *G = (grammar *)malloc(sizeof(grammar));
    tokenStream *s = (tokenStream*)malloc(sizeof(tokenStream));
    parseTree *t = (parseTree *)malloc(sizeof(parseTree));
    readGrammar("grammar.txt",G);
    tokeniseSourcecode("testcases/t3.txt",s);
    // printTokenStream(s);
    // printGrammar(G);
    createParseTree(t, s, G);
    // printf("\n%s\n", t->root->child->next->next->next->child->child->child->next->nodeData->nodeName);
    // printParseTree(t->root);
    typeExpressionTable *T=(typeExpressionTable *)malloc(sizeof(typeExpressionTable));
    T->capacity=10;
    T->count=0;
    T->dataTypeExpr=(tableData *)malloc(sizeof(tableData)*10);
    traverseParseTree(t, T);
    return 0;
}