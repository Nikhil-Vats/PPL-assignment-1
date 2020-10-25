#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parseTree.h"

parseTreeNode *rootNode;
void createParseTree(parseTree *t, tokenStream *s, grammar *G) {
    linkedList *firstRule = (linkedList *)malloc(sizeof(G->arr[0])); 
    firstRule = G->arr[0]; // get start rule

    // create root node with "START" symbol and make it root of parseTree with depth 0
    PTNodeData *nd = (PTNodeData *)malloc(sizeof(PTNodeData)); 
    nd->nodeName = malloc(strlen(firstRule->first->name) + 1);
    strcpy(nd->nodeName, firstRule->first->name);
    parseTreeNode *startNode = (parseTreeNode *)malloc(sizeof(parseTreeNode));
    startNode->nodeData = nd;
    startNode->depth = 0;
    startNode->next = NULL;
    startNode->child = NULL;
    t->root = startNode;
    rootNode = startNode;
    // printf("here");
    stack *st = (stack *)malloc(sizeof(stack));
    int insertCount = insertNodesInStack(st, G->arr[0]);
    printStack(st);
    tokenNode *currToken = s->first;
    bool prevNodeIsTerminal = false;
    bool res = moveForward(firstRule, currToken, st, startNode, G, prevNodeIsTerminal, insertCount);
    if(res) {
        printf("\nsuccess\n");
    } else {
        printf("\nfailure\n");
    }
}

bool moveForward(linkedList *rule, tokenNode *currToken, stack *st, parseTreeNode *prevNode, grammar *G, bool prevNodeIsTerminal, int flag) {
    // eg - PROGRAM PARENTHESES CURLYOP STATEMENTS CURLYCL
    if(isEmpty(st) && (currToken == NULL)) return true;
    while(!isEmpty(st)) {
        char *nodeName = malloc(strlen(st->top->name)+1);
        strcpy(nodeName,st->top->name);
        stackNode *topNode = st->top;
        // printf("%d\n", topNode->isTerminal);
        // if(strcmp(topNode->name,"LOGICALEXPR") == 0) {
        //     exit(1);
        // }
        if(strcmp(topNode->name, "epsilon") == 0) {
            pop(st);
        } else if(topNode->isTerminal == true) {
            if(strcasecmp(topNode->name, currToken->token) != 0) {
                // printf("\n%s != %s so we'll go backk\n", topNode->name, currToken->lexeme);
                return false;
            }
            // printf("\n%s == %s so we move on\n", topNode->name, currToken->lexeme);
            PTNodeData *nd = (PTNodeData *)malloc(sizeof(PTNodeData)); 
            nd->nodeName = malloc(strlen(currToken->lexeme)+1);
            strcpy(nd->nodeName, currToken->lexeme);

            parseTreeNode *ptNode = (parseTreeNode *)malloc(sizeof(parseTreeNode));
            ptNode->nodeData = nd;

            if(prevNodeIsTerminal == false) {
                // printf("\n ********* making %s the child of %s \n", ptNode->nodeData->nodeName, prevNode->nodeData->nodeName);
                prevNode->child = ptNode;
                ptNode->depth = prevNode->depth + 1;
            } else {
                // printf("\n  ********* making %s the sibling of %s \n", ptNode->nodeData->nodeName, prevNode->nodeData->nodeName);
                prevNode->next = ptNode;
                ptNode->depth = prevNode->depth;
            }
            
            currToken = currToken->next;
            prevNode = ptNode;
            prevNodeIsTerminal = true;
            pop(st);
            flag--;
        } else {
            // found a rule using currToken and G->arr[i].first
            PTNodeData *nd = (PTNodeData *)malloc(sizeof(PTNodeData)); 
            nd->nodeName = malloc(strlen(topNode->name)+1);
            strcpy(nd->nodeName, topNode->name);

            parseTreeNode *ptNode = (parseTreeNode *)malloc(sizeof(parseTreeNode));
            ptNode->nodeData = nd;

            if(prevNodeIsTerminal == false) {
                // printf("\n ********* making %s the child of %s \n", ptNode->nodeData->nodeName, prevNode->nodeData->nodeName);
                prevNode->child = ptNode;
                ptNode->depth = prevNode->depth + 1;
            } else {
                // printf("\n ********* making %s the sibling of %s \n", ptNode->nodeData->nodeName, prevNode->nodeData->nodeName);
                prevNode->next = ptNode;
                ptNode->depth = prevNode->depth;
            }
            
            prevNode = ptNode;
            prevNodeIsTerminal = false;
            grammar *expansionRules = findRules(st, G);
            // printf("\nexpansionRules.rulesCount = %d\n",expansionRules->rulesCount);
            for(int j = 0; j < expansionRules->rulesCount; j++) {
                // printf("\nusing rule %d\n",j);
                stack *newSt = copyStack(st);
                pop(newSt);
                int insertCount = insertNodesInStack(newSt, expansionRules->arr[j]);
                // printStack(newSt);
                bool res = moveForward(expansionRules->arr[j], currToken, newSt, ptNode, G, prevNodeIsTerminal, insertCount);
                if(res) {
                    return true;
                }
                if(st->top == NULL) {
                    return false;
                }
                // printf("\nbacktracking with %s and %s\n", currToken->lexeme, st->top->name);
                
                // removeNodesFromStack(st, insertCount);
                // printStack(st);
            }
            push(st, nodeName);
            return false;
        }
        // printStack(st);
    }
    if(isEmpty(st) && (currToken == NULL)) return true;
    return false;
}

stack* copyStack(stack *st) {
    stack *newOne = (stack *)malloc(sizeof(stack));
    stackNode *tempNode = st->top;
    // eg - PROGRAM PARENTHESES CURLYOP STATEMENTS CURLYCL
    char* temp[st->count-1];
    int i = 0;

    // create a temp array
    while(tempNode != NULL) {
        // printf("%s ", tempNode->name);
        temp[i] = malloc(strlen(tempNode->name)+1);
        temp[i++] = tempNode->name;
        tempNode = tempNode->next;
    }
    int count = i;
    // push using array in reverse direction (R->L) in stack to maintain leftmost derivation
    while(i > 0) {
        // printf("%s ", temp[--i]);
        push(newOne, temp[--i]);
    }
    // printStack(newOne);
    return newOne;
}

void printStack(stack *st) {
    // printf("here 2");
    stackNode *topNode = st->top;
    printf("\n---> ");
    // printf("%s ", topNode->name);
    while(topNode != NULL) {
        printf("<%s> ", topNode->name);
        topNode = topNode->next;
    }
    printf("\n");
}

grammar* findRules(stack *st, grammar *G) {
    stackNode *topNode = st->top;
    grammar *expansionRules = (grammar *)malloc(sizeof(grammar));
    int expansionRulesCount = 0;
    // printf("\%s\n",topNode->name);
    for(int i = 0; i < G->rulesCount; i++) {
        if(strcasecmp(topNode->name, G->arr[i]->first->name) == 0) {
            // printf("matched");
            expansionRules->arr[expansionRulesCount++] = G->arr[i];
        }
    }
    expansionRules->rulesCount = expansionRulesCount;
    return expansionRules;
}

int insertNodesInStack(stack *st, linkedList *rule) {
    node *tempNode = rule->first->next;

    // eg - PROGRAM PARENTHESES CURLYOP STATEMENTS CURLYCL
    char* temp[rule->count-1];
    int i = 0;

    // create a temp array
    while(tempNode != NULL) {
        // printf("%s ", tempNode->name);
        temp[i] = malloc(strlen(tempNode->name)+1);
        temp[i++] = tempNode->name;
        tempNode = tempNode->next;
    }
    int count = i;
    // push using array in reverse direction (R->L) in stack to maintain leftmost derivation
    while(i > 0) {
        // printf("%s ", temp[--i]);
        push(st, temp[--i]);
    }
    return count;
}

void removeNodesFromStack(stack *st, int insertCount) {
    // push using array in reverse direction (R->L) in stack to maintain leftmost derivation
    while(insertCount > 0) {
        // printf("deleting %d", insertCount);
        pop(st);
        insertCount--;
    }
}

void printParseTree(parseTreeNode *root) {
    if(root == NULL) {
        return;
    }
    parseTreeNode *tempNode = root;
    printf("\n\n\n\n\n\n***************");
    printf("*******%s*******\n",tempNode->nodeData->nodeName);
    printSibs(tempNode->next, tempNode->nodeData->nodeName);
    // printChild(tempNode->child, tempNode->nodeData->nodeName);
    printParseTree(tempNode->child);
    printParseTree(tempNode->next);
    printf("***************\n\n\n\n\n\n");
}

void printChild(parseTreeNode *root, char *parent) {
    if(root == NULL) return;
    printf("child of %s are -> ",parent);
    while(root) {
        printf("%s ->",root->nodeData->nodeName);
        root = root->child;
    }
}

void printSibs(parseTreeNode *root, char *origin) {
    if(root == NULL) return;
    printf("sibs of %s are -> ",origin);
    while(root) {
        printf("%s ->",root->nodeData->nodeName);
        root = root->next;
    }
}
