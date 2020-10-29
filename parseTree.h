#include <stdio.h>
#include <stdbool.h>
#include "traverseParseTree.h"
#include "typeExpressionTable.h"

#include "grammar.h"
#include "stack.h"
#include "tokenStream.h"

#ifndef PARSE_TREE_H
#define PARSE_TREE_H

typedef union typeExpression {
    struct TERMINAL terminal;
    struct DIMENSION dimension;
    struct DIMENSIONS dimensions;
    struct DIMENSIONS_REMAINING dimRemaining;
    struct DIMJAGGED dimJagged;
    struct RANGE range;
    struct NUMLIST numlist;
    struct NUMS nums;
    struct LISTOFNUMLIST listofnumlist;
    struct MAKEROWS makerows;
} typeExpression;

typedef struct PTNodeData {
    char *nodeName; // irrespective of terminal or non terminal
    int lineNo;
    int tETag;
    typeExpression *subExpression; // a union for storing type expression
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

void createParseTree(parseTree *t, tokenStream *s, grammar *G);
bool moveForward(linkedList *rule, tokenNode *currToken, stack *st, parseTreeNode *prevNode, grammar *G);
parseTreeNode* insertNodeInParseTree(stackNode *currSN, tokenNode *currTN, bool isTerminal);
void removeOldNodesFromParseTree(parseTreeNode *ptNode);

void printTypeExpressionTable(int T);

void printParseTree(parseTree *t);
void printChild(parseTreeNode *root, char *parent);
void printSibs(parseTreeNode *root, char *origin);

// ****************** TRAVERSAL FUNCTIONS BELOW ************************

void traverseParseTree(parseTree *t, typeExpressionTable *T);
void inOrder(parseTreeNode *n, typeExpressionTable *T);

void newPush(newStack *s, parseTreeNode *node);
void newPop(newStack *s);
bool newIsEmpty(newStack *s);
void pushID(parseTreeNode *p, newStack *s);
void findDimensions(parseTreeNode *p);
void findDimRemaining(parseTreeNode *p);
void findDimension(parseTreeNode *p);
void findDimJagged(parseTreeNode *node);
void findNums(parseTreeNode *node);
void findListofNumlist(parseTreeNode *node);
void findNumlist(parseTreeNode *node);
void findMakeRows(parseTreeNode *node);

void printDecErrors(int lineNo,int depth, char *message, int *optionalVar);

#endif