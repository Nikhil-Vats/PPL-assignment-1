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
    stack *st = (stack *)malloc(sizeof(stack));
    int insertCount = insertNodesInStack(st, G->arr[0], startNode, true);
    // free(firstRule);
    // printStack(st);
    tokenNode *currToken = s->first;
    // printf("%ld == %ld",sizeof(LISTOFNUMLIST),sizeof(subExpression));
    bool res = moveForward(firstRule, currToken, st, startNode, G);
    if(res) {
        printf("\nsuccess\n");
    } else {
        printf("\nfailure\n");
    }
}

bool moveForward(linkedList *rule, tokenNode *currToken, stack *st, parseTreeNode *prevNode, grammar *G) {
    // eg - PROGRAM PARENTHESES CURLYOP STATEMENTS CURLYCL
    if(isEmpty(st) && (currToken == NULL)) return true;
    while(!isEmpty(st)) {
        char *nodeName = malloc(strlen(st->top->name)+1);
        strcpy(nodeName,st->top->name);
        stackNode *topNode = st->top;
        if(strcmp(topNode->name, "epsilon") == 0) {
            pop(st);
        } else if(topNode->isTerminal == true) {
            if(strcasecmp(topNode->name, currToken->token) != 0) {
                printf("\n%s != %s so we'll go backk\n", topNode->name, currToken->lexeme);
                return false;
            }
            printf("\n%s == %s so we move on\n", topNode->name, currToken->lexeme);

            parseTreeNode* insertedNode = insertNodeInParseTree(topNode, currToken, true);
            currToken = currToken->next;
            prevNode = insertedNode;
            // printStack(st);
            pop(st);
            // return moveForward(rule, currToken, st, prevNode, G);
        } else {
            parseTreeNode *insertedNode = insertNodeInParseTree(topNode, currToken, false);
            prevNode = insertedNode;
            grammar *expansionRules = findRules(st, G);
            for(int j = 0; j < expansionRules->rulesCount; j++) {
                stack *newSt = copyStack(st);
                pop(newSt);
                int insertCount = insertNodesInStack(newSt, expansionRules->arr[j], prevNode, true);
                printStack(newSt);
                // exit(1);
                bool res = moveForward(expansionRules->arr[j], currToken, newSt, prevNode, G);
                if(res) {
                    return true;
                }
                if(st->top == NULL) {
                    return false;
                }
                printf("before empty stack\n");
                // emptyStack(newSt);
                // free(newSt);
                printf("remove old nodes\n");
                removeOldNodesFromParseTree(prevNode);
                // printStack(st);
            }
            // for(int k = 0; k < expansionRules->rulesCount; k++) {
            //     expansionRules->arr[k] = NULL;
            // }
            // free(expansionRules);
            return false;
        }
        printStack(st);
    }
    if(isEmpty(st) && (currToken == NULL)) return true;
    return false;
}

void removeOldNodesFromParseTree(parseTreeNode *ptNode) {
    parseTreeNode *child = ptNode->child;
    if(child == NULL) {
        // printf("\nno child for ** %s ** \n", ptNode->nodeData->nodeName);
        return;
    }
    // statements
    // decstmt -> assstmt
    // dec -> var -> of -> type -> : -> datatype,    some unnecessary nodes of assstms
    // childs of dec 1 -> 2 -> 3
    while(child != NULL) {
        removeOldNodesFromParseTree(child);
        parseTreeNode *temp = child;
        child = child->next;

        PTNodeData *dataToDelete = temp->nodeData;
        // free(dataToDelete->subExpression.terminal.type);
        printf("\njust going to delete ** %s ** \n", temp->nodeData->nodeName);
        // free(dataToDelete->subExpression);
        // free(dataToDelete->nodeName);
        free(dataToDelete);
        // dataToDelete->nodeName = NULL;
        temp->nodeData = NULL;
        temp->next = NULL;
        temp->child = NULL;
        // temp = NULL;
        // free(temp);
        // printf("\n ** ** deletion should be complete now ** ** \n");
    }
    free(ptNode->child);
    ptNode->child = NULL;
    return;
}

parseTreeNode* insertNodeInParseTree(stackNode *currSN, tokenNode *currTN, bool isTerminal) {
    parseTreeNode *parentNode = currSN->parentNode;
    
    PTNodeData *nd = (PTNodeData *)malloc(sizeof(PTNodeData));
    // MAKEROWS makeRows = (MAKEROWS)malloc()
    // nd->subExpression.makerows = makeRows;
    // nd->subExpression.listofnumlist.count = 0;
    // nd->subExpression.listofnumlist.count = 0;
    // nd->subExpression = (typeExpression *)malloc(sizeof(typeExpression));
    nd->subExpression = NULL;
    // for(int j = 0; j < 10; j++) {
    //     nd->subExpression.listofnumlist.listofnums[j].count = 0;
    //     for(int i = 0; i < 20; i++) {
    //         nd->subExpression.listofnumlist.listofnums[j].num[i] = 0;
    //     }
    // }
    if(isTerminal) {
        nd->nodeName = malloc(strlen(currTN->lexeme)+1);
        nd->lineNo = currTN->lineNo;
        strcpy(nd->nodeName, currTN->lexeme);
        // printf("\ncreated node for %s \n", nd->nodeName);
    } else {
        nd->lineNo = -1;
        nd->nodeName = malloc(strlen(currSN->name)+1);
        strcpy(nd->nodeName, currSN->name);
        // printf("\ncreated node for %s \n", currSN->name);
    }

    parseTreeNode *nodeToInsert = (parseTreeNode *)malloc(sizeof(parseTreeNode));
    nodeToInsert->nodeData = nd;
    printf("\ncreated node for %s \n", nodeToInsert->nodeData->nodeName);
    // eg - PROGRAM PARENTHESES CURLYOP STATEMENTS CURLYCL
    if(parentNode->child == NULL) {
        parentNode->child = nodeToInsert;
        printf("it's a child of %s\n", parentNode->nodeData->nodeName);
    } else {
        // printf("not a child");
        printf("parent (%s) already has a child \n", parentNode->nodeData->nodeName);
        parentNode = parentNode->child;
        while(parentNode->next != NULL) {
            parentNode = parentNode->next;
        }
        printf("it's a next of %s\n", parentNode->nodeData->nodeName);
        parentNode->next = nodeToInsert;
    }
    return nodeToInsert;
}

stack* copyStack(stack *st) {
    stack *newOne = (stack *)malloc(sizeof(stack));
    stackNode *tempNode = st->top;
    int i = 0;

    while(tempNode->next != NULL) {
        tempNode = tempNode->next;
    }
    int count = i;
    // push using array in reverse direction (R->L) in stack to maintain leftmost derivation
    while(tempNode != NULL) {
        i = i - 1;
        push(newOne, tempNode->name, tempNode->parentNode, tempNode->isChild);
        tempNode = tempNode->prev;
    }
    // printStack(newOne);
    return newOne;
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

int insertNodesInStack(stack *st, linkedList *rule, parseTreeNode *parent, bool isChild) {
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
        // printf("\n%s is the name of parent\n", parent->nodeData->nodeName);
        push(st, temp[--i], parent, true);
    }
    // int delCount = count;
    // while(delCount > 0) {
    //     free(temp[--delCount]);
    // }
    return count;
}

// functions below are just for testing and/or old functions not useful anymore

void printStack(stack *st) {
    // printf("here 2");
    stackNode *topNode = st->top;
    printf("\n---> ");
    // printf("%s ", topNode->name);
    if(topNode) {
        while(topNode != NULL) {
            printf("<%s> ", topNode->name);
            // printf("<%s> (%s) ", topNode->name, topNode->parentNode->nodeData->nodeName);
            topNode = topNode->next;
        }
    } else {
        printf("stack empty");
    }
    printf("\n");
}

void removeNodesFromStack(stack *st, int insertCount) {
    // push using array in reverse direction (R->L) in stack to maintain leftmost derivation
    while(insertCount > 0) {
        // printf("deleting %d", insertCount);
        pop(st);
        insertCount--;
    }
}

void printParseTree(parseTree *t) {
    printf("************************************************************");
    printf("\n\n\ninput is 3 printParseTree is called\n\n\n");
    printf("************************************************************");
    // parseTreeNode *root = t->root;
    // if(root == NULL) {
    //     return;
    // }
    // parseTreeNode *tempNode = root;
    // printf("\n\n\n\n\n\n***************");
    // printf("*******%s*******\n",tempNode->nodeData->nodeName);
    // printSibs(tempNode->next, tempNode->nodeData->nodeName);
    // // printChild(tempNode->child, tempNode->nodeData->nodeName);
    // printParseTree(tempNode->child);
    // printParseTree(tempNode->next);
    // printf("***************\n\n\n\n\n\n");
}

// void traverseParseTree(parseTree *t, int T) {
//     printf("************************************************************");
//     printf("\n\n\ninput is %d traverseParseTree is called\n\n\n", T);
//     printf("************************************************************");
// }
void printTypeErrors(parseTree *t, int T) {
    printf("************************************************************");
    printf("\n\n\ninput is %d printTypeErrors is called\n\n\n", T);
    printf("************************************************************");
}
void printTypeExpressionTable(int T) {
    printf("************************************************************");
    printf("\n\n\ninput is %d printTypeExpressionTable is called\n\n\n", T);
    printf("************************************************************");
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
