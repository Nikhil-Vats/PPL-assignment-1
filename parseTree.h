#include <stdio.h>
#include <stdbool.h>

#include "tokenStream.h"
#include "grammar.h"
#include "stack.h"

typedef struct PTNodeData {
    char *nodeName; // irrespective of terminal or non terminal
    // a union for storing type expression in case of identifiers
} PTNodeData;

typedef struct parseTreeNode {
    PTNodeData *nodeData;
    int depth;
    struct parseTreeNode *child;
    struct parseTreeNode *next;
} parseTreeNode;

typedef struct parseTree {
    parseTreeNode *root;
} parseTree;

void printStack(stack *st);
grammar* findRules(stack *st, grammar *G);
void createParseTree(parseTree *t, tokenStream *s, grammar *G);
int insertNodesInStack(stack *st, linkedList *rule);
void removeNodesFromStack(stack *st, int insertCount);
void moveForward(linkedList *rule, tokenNode *currToken, stack *st, parseTreeNode *prevNode, grammar *G, bool prevNodeIsTerminal, int flag);
void printChild(parseTreeNode *root, char *parent);
void printSibs(parseTreeNode *root, char *origin);
void printParseTree(parseTreeNode *root);