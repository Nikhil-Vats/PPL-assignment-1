#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parseTree.h"

parseTreeNode *rootNode;
void createParseTree(parseTree *t, tokenStream *s, grammar *G)
{
    linkedList *firstRule = (linkedList *)calloc(1, sizeof(G->arr[0]));
    firstRule = G->arr[0]; // get start rule

    // create root node with "START" symbol and make it root of parseTree with depth 0
    PTNodeData *nd = (PTNodeData *)calloc(1, sizeof(PTNodeData));
    nd->nodeName = calloc(1, strlen(firstRule->first->name) + 1);
    strcpy(nd->nodeName, firstRule->first->name);
    parseTreeNode *startNode = (parseTreeNode *)calloc(1, sizeof(parseTreeNode));
    startNode->nodeData = nd;
    startNode->depth = 0;
    startNode->next = NULL;
    startNode->child = NULL;
    t->root = startNode;
    rootNode = startNode;
    stack *st = (stack *)calloc(1, sizeof(stack));
    int insertCount = insertNodesInStack(st, G->arr[0], startNode, true);
    tokenNode *currToken = s->first;
    bool res = moveForward(firstRule, currToken, st, startNode, G);
    if (res)
    {
        printf("\n*************************************************\n");
        printf("\nParse tree is created successfully!\n");
        printf("\n*************************************************\n");
    }
    else
    {
        printf("\n*************************************************\n");
        printf("\nParse tree creation FAILED!\n");
        printf("\n*************************************************\n");
    }
}

bool moveForward(linkedList *rule, tokenNode *currToken, stack *st, parseTreeNode *prevNode, grammar *G)
{
    if (isEmpty(st) && (currToken == NULL))
        return true;

    while (!isEmpty(st))
    {
        char *nodeName = calloc(1, strlen(st->top->name) + 1);
        strcpy(nodeName, st->top->name);
        stackNode *topNode = st->top;
        if (strcmp(topNode->name, "epsilon") == 0)
        {
            pop(st);
        }
        else if (topNode->isTerminal == true)
        {
            if (strcasecmp(topNode->name, currToken->token) != 0)
            {
                // printf("\n%s != %s so we'll go backk\n", topNode->name, currToken->lexeme);
                return false;
            }
            // printf("\n%s == %s so we move on\n", topNode->name, currToken->lexeme);

            parseTreeNode *insertedNode = insertNodeInParseTree(topNode, currToken, true);
            currToken = currToken->next;
            prevNode = insertedNode;
            pop(st);
        }
        else
        {
            parseTreeNode *insertedNode = insertNodeInParseTree(topNode, currToken, false);
            prevNode = insertedNode;
            grammar *expansionRules = findRules(st, G);
            for (int j = 0; j < expansionRules->rulesCount; j++)
            {
                stack *newSt = copyStack(st);
                pop(newSt);
                int insertCount = insertNodesInStack(newSt, expansionRules->arr[j], prevNode, true);
                // printStack(newSt);
                bool res = moveForward(expansionRules->arr[j], currToken, newSt, prevNode, G);
                if (res)
                {
                    return true;
                }
                if (st->top == NULL)
                {
                    return false;
                }
                emptyStack(newSt);
                free(newSt);
                removeOldNodesFromParseTree(prevNode);
            }
            for (int k = 0; k < expansionRules->rulesCount; k++)
            {
                expansionRules->arr[k] = NULL;
            }
            free(expansionRules);
            return false;
        }
        // printStack(st);
    }
    if (isEmpty(st) && (currToken == NULL))
        return true;
    return false;
}

void removeOldNodesFromParseTree(parseTreeNode *ptNode)
{
    parseTreeNode *child = ptNode->child;
    if (child == NULL)
    {
        return;
    }
    while (child != NULL)
    {
        removeOldNodesFromParseTree(child);
        parseTreeNode *temp = child;
        child = child->next;

        PTNodeData *dataToDelete = temp->nodeData;
        // printf("\njust going to delete ** %s ** \n", temp->nodeData->nodeName);
        free(dataToDelete->subExpression);
        free(dataToDelete->nodeName);
        free(dataToDelete);
        temp->nodeData = NULL;
        temp->next = NULL;
        temp->child = NULL;
    }
    free(ptNode->child);
    ptNode->child = NULL;
    return;
}

parseTreeNode *insertNodeInParseTree(stackNode *currSN, tokenNode *currTN, bool isTerminal)
{
    parseTreeNode *parentNode = currSN->parentNode;

    PTNodeData *nd = (PTNodeData *)calloc(1, sizeof(PTNodeData));
    nd->subExpression = (typeExpression *)calloc(1, sizeof(typeExpression));
    if (isTerminal)
    {
        nd->nodeName = calloc(1, strlen(currTN->lexeme) + 1);
        nd->lineNo = currTN->lineNo;
        strcpy(nd->nodeName, currTN->lexeme);
    }
    else
    {
        nd->lineNo = currTN->lineNo;
        nd->nodeName = calloc(1, strlen(currSN->name) + 1);
        strcpy(nd->nodeName, currSN->name);
    }

    parseTreeNode *nodeToInsert = (parseTreeNode *)calloc(1, sizeof(parseTreeNode));
    nodeToInsert->nodeData = nd;
    nodeToInsert->depth = parentNode->depth + 1;
    // printf("\ncreated node for %s \n", nodeToInsert->nodeData->nodeName);
    if (parentNode->child == NULL)
    {
        parentNode->child = nodeToInsert;
    }
    else
    {
        // printf("not a child");
        parentNode = parentNode->child;
        while (parentNode->next != NULL)
        {
            parentNode = parentNode->next;
        }
        // printf("it's a next of %s\n", parentNode->nodeData->nodeName);
        parentNode->next = nodeToInsert;
    }
    return nodeToInsert;
}

stack *copyStack(stack *st)
{
    stack *newOne = (stack *)calloc(1, sizeof(stack));
    stackNode *tempNode = st->top;
    int i = 0;

    while (tempNode->next != NULL)
    {
        tempNode = tempNode->next;
    }
    int count = i;
    // push using array in reverse direction (R->L) in stack to maintain leftmost derivation
    while (tempNode != NULL)
    {
        i = i - 1;
        push(newOne, tempNode->name, tempNode->parentNode, tempNode->isChild);
        tempNode = tempNode->prev;
    }
    return newOne;
}

grammar *findRules(stack *st, grammar *G)
{
    stackNode *topNode = st->top;
    grammar *expansionRules = (grammar *)calloc(1, sizeof(grammar));
    int expansionRulesCount = 0;
    for (int i = 0; i < G->rulesCount; i++)
    {
        if (strcasecmp(topNode->name, G->arr[i]->first->name) == 0)
        {
            expansionRules->arr[expansionRulesCount++] = G->arr[i];
        }
    }
    expansionRules->rulesCount = expansionRulesCount;
    return expansionRules;
}

int insertNodesInStack(stack *st, linkedList *rule, parseTreeNode *parent, bool isChild)
{
    node *tempNode = rule->first->next;

    char *temp[rule->count - 1];
    int i = 0;

    while (tempNode != NULL)
    {
        temp[i] = calloc(1, strlen(tempNode->name) + 1);
        temp[i++] = tempNode->name;
        tempNode = tempNode->next;
    }
    int count = i;
    // push using array in reverse direction (R->L) in stack to maintain leftmost derivation
    while (i > 0)
    {
        // printf("\n%s is the name of parent\n", parent->nodeData->nodeName);
        push(st, temp[--i], parent, true);
    }
    return count;
}

// functions below are just for testing and/or old functions not useful anymore

void printStack(stack *st)
{
    stackNode *topNode = st->top;
    printf("\n---> ");
    if (topNode)
    {
        while (topNode != NULL)
        {
            printf("<%s> ", topNode->name);
            topNode = topNode->next;
        }
    }
    else
    {
        printf("stack empty");
    }
    printf("\n");
}

void printParseTree(parseTree *t)
{
    parseTreeNode *root = t->root;
    // printf("")
    printfunc(root);
}

void printfunc(parseTreeNode *root)
{
    if (root == NULL)
    {
        return;
    }
    printf("\n%s\n", root->nodeData->nodeName);
    printAll(root);
    printfunc(root->child);
    printfunc(root->next);
}
// /*  Symbol name
//  Whether terminal or non terminal
//  Type expression stored in the corresponding node (if non-leaf)
//  Name of lexeme (if leaf node)
//  Line number (if leaf node)
//  Grammar rule applied for expansion of this node while parsing (if non leaf)
//  Depth of node (root of the parse tree at depth 0) */
void printAll(parseTreeNode *root)
{
    char *ter;
    char *field;
    if (isTerminal(root->nodeData->nodeName))
    {
        ter = "Terminal";
        printf("Symbol - %-5s | %-5s | Type- *** | lexeme-%-5s | Line-%-5d | rule- *** | depth-%-5d ", getTokenName(root->nodeData->nodeName), ter, root->nodeData->nodeName, root->nodeData->lineNo, root->depth);
    }
    else
    {
        ter = "Non Terminal";

        if (strcmp(root->nodeData->nodeName, "DIMENSIONS") == 0)
        {
            printf("Symbol - %-5s | %-5s | Type-<DIMENSIONS, Number of Dimensions=%-5d> | lexeme- *** | Line- *** | depth-%-5d ", root->nodeData->nodeName, ter, root->nodeData->subExpression->dimensions.count, root->depth);
        }
        else if (strcmp(root->nodeData->nodeName, "DIMENSION") == 0)
        {
            printf("Symbol - %-5s | %-5s | Type-<DIMENSION, Range=(%-5s,%-5s)> | lexeme- *** | Line- *** | depth-%-5d ", root->nodeData->nodeName, ter, root->nodeData->subExpression->dimension.range[0], root->nodeData->subExpression->dimension.range[1], root->depth);
        }
        else if (strcmp(root->nodeData->nodeName, "DIMENSIONS_REMAINING") == 0)
        {
            printf("Symbol - %-5s | %-5s | Type-<DIMENSIONS_REMAINING Number of Dimensions=%-5d> | lexeme- *** | Line- ***  | depth-%-5d ", root->nodeData->nodeName, ter, root->nodeData->subExpression->dimRemaining.count, root->depth);
        }
        else if (strcmp(root->nodeData->nodeName, "DIMJAGGED") == 0)
        {
            printf("Symbol - %-5s | %-5s | Type-<DIMJAGGED Number of Dimensions=%-5d Ranges=(%-5s,%-5s)> | lexeme- *** | Line- *** | depth-%-5d ", root->nodeData->nodeName, ter, root->nodeData->subExpression->dimJagged.dim, root->nodeData->subExpression->dimJagged.num1, root->nodeData->subExpression->dimJagged.num2, root->depth);
        }
        else if (strcmp(root->nodeData->nodeName, "RANGE") == 0)
        {
            printf("Symbol - %-5s | %-5s | Type-<Range Range=(%-5s,%-5s)> | lexeme- *** | Line- *** | depth-%-5d ", root->nodeData->nodeName, ter, root->nodeData->subExpression->range.num1, root->nodeData->subExpression->range.num2, root->depth);
        }
        else if (strcmp(root->nodeData->nodeName, "NUMLIST") == 0)
        {
            printf("Symbol - %-5s | %-5s | Type-<NUMLIST Number of Integers=%-5d> | lexeme- *** | Line- *** | depth-%-5d ", root->nodeData->nodeName, ter, root->nodeData->subExpression->numlist.count, root->depth);
        }
        else if (strcmp(root->nodeData->nodeName, "NUMS") == 0)
        {
            printf("Symbol - %-5s | %-5s | Type-<NUMS Number of Integers=%-5d> | lexeme- *** | Line- ***  | depth-%-5d ", root->nodeData->nodeName, ter, root->nodeData->subExpression->nums.count, root->depth);
        }
        else if (strcmp(root->nodeData->nodeName, "LISTOFNUMLIST") == 0)
        {
            printf("Symbol - %-5s | %-5s | Type-<LISTOFNUMLIST Number of Integers=%-5d> | lexeme- *** | Line- ***  | depth-%-5d ", root->nodeData->nodeName, ter, root->nodeData->subExpression->listofnumlist.count, root->depth);
        }
        else if (strcmp(root->nodeData->nodeName, "MAKEROWS") == 0)
        {
            printf("Symbol - %-5s | %-5s | Type-<MAKEROWS Number of Rows=%-5d 2D or 3D=%-5s> | lexeme- *** | Line- ***  | depth-%-5d ", root->nodeData->nodeName, ter, root->nodeData->subExpression->makerows.rows, root->nodeData->subExpression->makerows.is3D == 0 ? "2D" : "3D", root->depth);
        }
        else if(strcmp(root->nodeData->nodeName, "START") != 0) {
            if (root->nodeData->subExpression->data.table.field2 == primitive)
            {
                if (strcmp(root->nodeData->subExpression->data.table.field4.typePrimitive, "integer") == 0)
                {
                    field = "Primitive-Integer";
                }
                else if (strcmp(root->nodeData->subExpression->data.table.field4.typePrimitive, "real") == 0)
                {
                    field = "Primitive-Real";
                }
                else if (strcmp(root->nodeData->subExpression->data.table.field4.typePrimitive, "boolean") == 0)
                {
                    field = "Primitive-Boolean";
                }
            }
            else if (root->nodeData->subExpression->data.table.field2 == rectangular)
            {
                field = "RectangularArray";
            }
            else if (root->nodeData->subExpression->data.table.field2 == jagged)
            {
                field = "JaggedArray";
            }
        }
        if (strcmp(root->nodeData->nodeName, "ARITHEXPR") == 0)
        {
            printf("Symbol - %-5s | %-5s | Type-<ARITHEXPR Datatype=%-5s> | lexeme- *** | Line- ***  | depth-%-5d ", root->nodeData->nodeName, ter, field, root->depth);
        }
        else if (strcmp(root->nodeData->nodeName, "ARITHEXPR2") == 0)
        {
            printf("Symbol - %-5s | %-5s | Type-<ARITHEXPR2 Datatype=%-5s> | lexeme- *** | Line- ***  | depth-%-5d ", root->nodeData->nodeName, ter, field, root->depth);
        }
        else if (strcmp(root->nodeData->nodeName, "VARNUM") == 0)
        {
            printf("Symbol - %-5s | %-5s | Type-<VARNUM Datatype=%-5s> | lexeme- *** | Line- ***  | depth-%-5d ", root->nodeData->nodeName, ter, field, root->depth);
        }
        else if (strcmp(root->nodeData->nodeName, "VAR") == 0)
        {
            printf("Symbol - %-5s | %-5s | Type-<VAR Datatype=%-5s> | lexeme- *** | Line- ***  | depth-%-5d ", root->nodeData->nodeName, ter, field, root->depth);
        }
        else if (strcmp(root->nodeData->nodeName, "LOGICALEXPR") == 0)
        {
            printf("Symbol - %-5s | %-5s | Type-<LOGICALEXPR Datatype=%-5s> | lexeme- *** | Line- ***  | depth-%-5d ", root->nodeData->nodeName, ter, field, root->depth);
        }
        else if (strcmp(root->nodeData->nodeName, "LOGICALEXPR2") == 0)
        {
            printf("Symbol - %-5s | %-5s | Type-<LOGICALEXPR2 Datatype=%-5s> | lexeme- *** | Line- ***  | depth-%-5d ", root->nodeData->nodeName, ter, field, root->depth);
        }
    }
}

void printChild(parseTreeNode *root, char *parent)
{
    if (root == NULL)
        return;
    printf("child of %s are -> ", parent);
    while (root)
    {
        printf("%s ->", root->nodeData->nodeName);
        root = root->child;
    }
}

void printSibs(parseTreeNode *root, char *origin)
{
    if (root == NULL)
        return;
    printf("sibs of %s are -> ", origin);
    while (root)
    {
        printf("%s ->", root->nodeData->nodeName);
        root = root->next;
    }
}
