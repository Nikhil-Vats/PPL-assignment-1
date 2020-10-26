#include <stdio.h>
#include <stdbool.h>

// #include "grammar.h"
// #include "tokenStream.h"

// #ifndef PARSE_TREE_H
// #define PARSE_TREE_H
typedef struct PTNodeData {
    char *nodeName; // irrespective of terminal or non terminal
    int lineNo;
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

// #endif