#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"

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
    int insertCount = insertNodesInStack(st, G->arr[0], startNode, true);
    free(firstRule);
    // printStack(st);
    tokenNode *currToken = s->first;
    bool res = moveForward(firstRule, currToken, st, startNode, G);
    if(res) {
        printf("\nsuccess\n");
    } else {
        printf("\nfailure\n");
    }
}

bool moveForward(linkedList *rule, tokenNode *currToken, stack *st, parseTreeNode *prevNode, grammar *G) {
    // eg - PROGRAM PARENTHESES CURLYOP STATEMENTS CURLYCL
    // printf("\nat 38 in move forward\n");
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
                printf("\n%s != %s so we'll go backk\n", topNode->name, currToken->lexeme);
                return false;
            }
            printf("\n%s == %s so we move on\n", topNode->name, currToken->lexeme);
            // PTNodeData *nd = (PTNodeData *)malloc(sizeof(PTNodeData)); 
            // nd->nodeName = malloc(strlen(currToken->lexeme)+1);
            // strcpy(nd->nodeName, currToken->lexeme);

            // parseTreeNode *ptNode = (parseTreeNode *)malloc(sizeof(parseTreeNode));
            // ptNode->nodeData = nd;

            // if(prevNodeIsTerminal == false) {
            //     // printf("\n ********* making %s the child of %s \n", ptNode->nodeData->nodeName, prevNode->nodeData->nodeName);
            //     prevNode->child = ptNode;
            //     ptNode->depth = prevNode->depth + 1;
            // } else {
            //     // printf("\n  ********* making %s the sibling of %s \n", ptNode->nodeData->nodeName, prevNode->nodeData->nodeName);
            //     prevNode->next = ptNode;
            //     ptNode->depth = prevNode->depth;
            // }
            // printf("\nat 72 in move forward while\n");
            parseTreeNode* insertedNode = insertNodeInParseTree(topNode, currToken, true);
            currToken = currToken->next;
            prevNode = insertedNode;
            // printStack(st);
            pop(st);
        } else {
            // found a rule using currToken and G->arr[i].first
            // PTNodeData *nd = (PTNodeData *)malloc(sizeof(PTNodeData)); 
            // nd->nodeName = malloc(strlen(topNode->name)+1);
            // strcpy(nd->nodeName, topNode->name);

            parseTreeNode *insertedNode = insertNodeInParseTree(topNode, currToken, false);
            prevNode = insertedNode;
            // ptNode->nodeData = nd;
            // printf("%s is the topNode and its parent name is %s\n", topNode->name, topNode->parentNode->nodeData->nodeName);
            // if(prevNodeIsTerminal == false) {
            //     // printf("\n ********* making %s the child of %s \n", ptNode->nodeData->nodeName, prevNode->nodeData->nodeName);
            //     prevNode->child = ptNode;
            //     ptNode->depth = prevNode->depth + 1;
            // } else {
            //     // printf("\n ********* making %s the sibling of %s \n", ptNode->nodeData->nodeName, prevNode->nodeData->nodeName);
            //     prevNode->next = ptNode;
            //     ptNode->depth = prevNode->depth;
            // }
            
            grammar *expansionRules = findRules(st, G);
            // printf("\nexpansionRules.rulesCount = %d\n",expansionRules->rulesCount);
            for(int j = 0; j < expansionRules->rulesCount; j++) {
                // printf("\nusing rule %d\n",j);
                stack *newSt = copyStack(st);
                // printf("\nat 107 in move forward while else loop\n");
                pop(newSt);
                int insertCount = insertNodesInStack(newSt, expansionRules->arr[j], prevNode, true);
                printStack(newSt);
                // eg - PROGRAM PARENTHESES CURLYOP STATEMENTS CURLYCL
                bool res = moveForward(expansionRules->arr[j], currToken, newSt, prevNode, G);
                if(res) {
                    return true;
                }
                if(st->top == NULL) {
                    return false;
                }
                // emptyStack(newSt);
                // free(newSt);
                // printf("\nbacktracking with %s and %s\n", currToken->lexeme, st->top->name);
                // removeOldNodesFromParseTree(prevNode);
                // removeNodesFromStack(st, insertCount);
                // printStack(st);
            }
            // free(expansionRules);
            // push(st, nodeName, prevNode,true);
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
        printf("\njust going to delete ** %s ** \n", temp->nodeData->nodeName);
        temp->nodeData = NULL;
        if(dataToDelete != NULL) {
            // free(dataToDelete);
        }
        if(temp != NULL) {
            // free(temp);
        }
        temp->next = NULL;
        temp->child = NULL;
        
        // printf("\n ** ** deletion should be complete now ** ** \n");
    }
    return;
}

parseTreeNode* insertNodeInParseTree(stackNode *currSN, tokenNode *currTN, bool isTerminal) {
    parseTreeNode *parentNode = currSN->parentNode;
    
    PTNodeData *nd = (PTNodeData *)malloc(sizeof(PTNodeData)); 
    if(isTerminal) {
        nd->nodeName = malloc(strlen(currTN->lexeme)+1);
        nd->lineNo = currTN->lineNo;
        strcpy(nd->nodeName, currTN->lexeme);
        printf("\ncreated node for %s \n", currTN->lexeme);
    } else {
        nd->lineNo = -1;
        printf("\ncreated node for %s \n", currSN->name);
        nd->nodeName = malloc(strlen(currSN->name)+1);
        strcpy(nd->nodeName, currSN->name);
    }

    parseTreeNode *nodeToInsert = (parseTreeNode *)malloc(sizeof(parseTreeNode));
    nodeToInsert->nodeData = nd;

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

    // parseTreeNode **parents = (parseTreeNode **)malloc(sizeof(parseTreeNode *)*st->count-1);

    // create a temp array
    while(tempNode->next != NULL) {
        // printf("%s ", tempNode->name);
        // temp[i] = malloc(strlen(tempNode->name)+1);
        // temp[i] = tempNode->name;
        // // parents[i] = (parseTreeNode *)malloc(sizeof(parseTreeNode));
        // parents[i++] = tempNode->parentNode;
        tempNode = tempNode->next;
    }
    int count = i;
    // push using array in reverse direction (R->L) in stack to maintain leftmost derivation
    while(tempNode != NULL) {
        i = i - 1;
        // printf("%s [%s]", temp[i], parents[i]->nodeData->nodeName);
        push(newOne, tempNode->name, tempNode->parentNode, tempNode->isChild);
        tempNode = tempNode->prev;
        // push(st, temp[--i], parent, true);
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

void removeNodesFromStack(stack *st, int insertCount) {
    // push using array in reverse direction (R->L) in stack to maintain leftmost derivation
    while(insertCount > 0) {
        // printf("deleting %d", insertCount);
        pop(st);
        insertCount--;
    }
}

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
