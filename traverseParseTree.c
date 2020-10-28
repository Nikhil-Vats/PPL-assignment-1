#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parseTree.h"

void traverseParseTree(parseTree *t, typeExpressionTable *T)
{
    printf("\n%s is the name of root \n",t->root->child->next->next->next->nodeData->nodeName);

    if (t->root != NULL)
    {
        inOrder(t->root->child->next->next->next->next, T);
    }
}
// START program PARENTHESES CURLYOP STATEMENTS CURLYCL
// STATEMENTS DECSTMTS ASSIGNSTMTS
// DECSTMTS DECSTMT MOREDEC
// DECLARATIONSTATEMENT declare list of variables IDLIST colon DATATYPE semicol
void inOrder(parseTreeNode *n, typeExpressionTable *T)
{
    if (n != NULL)
    {   if (strcmp(n->nodeData->nodeName, "STATEMENTS") == 0) {
            printf("\n%s is the name of root \n",n->nodeData->nodeName);
            n = n->child;
        }
        if(strcmp(n->nodeData->nodeName, "DECLARATIONSTATEMENTS") == 0) {
            printf("\n%s is the name of root \n",n->nodeData->nodeName);
            n = n->child;
        }
        if(strcmp(n->nodeData->nodeName, "MOREDECLARATIONSTATEMENTS") == 0) {
            printf("\n%s is the name of root \n",n->nodeData->nodeName);
            n = n->child;
        }
        if (strcmp(n->nodeData->nodeName, "DECLARATIONSTATEMENT") == 0)
        {
            parseTreeNode *children = n->child;
            // parseTreeNode *col = NULL;
            newStack *sta = (newStack *)malloc(sizeof(newStack));
            // nd->subExpression = (typeExpression *)malloc(sizeof(typeExpression));
            // children->nodeData->subExpression = (typeExpression *)malloc(sizeof(typeExpression));
            // typeExpression *sub = (typeExpression *)malloc(sizeof(typeExpression));
            // COLON followed by Datatype(Primitive) / jagged / array(Rectangular)
            // COLON preceeded by ID/IDLIST
            // Array/jagged followed by DIMENSIONS/DIMJAGGED, counter for no of dimensions
            printf("\n before while at 39 %s\n", children->nodeData->nodeName);
            int flag = 1;
            tableData data;
            while (children && flag)
            {
                // printf("\n in children loop at 42 %s\n", children->nodeData->nodeName);
                if (isTerminal(children->nodeData->nodeName))
                {
                    // printf("\n this is a terminal %s\n", children->nodeData->nodeName);
                    if (strcmp(getTokenName(children->nodeData->nodeName), "ID") == 0)
                    {
                        printf("\n this is an identifier %s\n", children->nodeData->nodeName);
                        newPush(sta, children);
                        // children->nodeData->subExpression->terminal.type = children->nodeData->nodeName;
                    }
                    else if (strcmp(children->nodeData->nodeName, ":") == 0)
                    {
                        printf("\n this is a colon %s\n", children->nodeData->nodeName);
                        // col = (parseTreeNode *)malloc(sizeof(parseTreeNode));
                        // col = children;
                        // flag = 0;
                    }
                    else if (strcmp(children->nodeData->nodeName, ";") == 0)
                    {
                        printf("\n this is a semicolon %s\n", children->nodeData->nodeName);
                        // col = (parseTreeNode *)malloc(sizeof(parseTreeNode));
                        // col = children;
                        flag = 0;
                    }
                    else
                    {
                        printf("\n this is %s\n", children->nodeData->nodeName);
                        // populate the type expression/lexeme of the terminals in PTNodeData
                        // children->nodeData->subExpression->terminal;
                        // children->nodeData->subExpression = (typeExpression *)malloc(sizeof(typeExpression));
                        // children->nodeData->subExpression->terminal.type = ;
                        // printf("\n this is idk what 2 %s\n", children->nodeData->nodeName);
                        children->nodeData->subExpression->terminal.type = children->nodeData->nodeName;
                        // printf("\n this is idk what 3 %s\n", children->nodeData->nodeName);
                    }
                }
                //IDLIST
                else if (strcmp(children->nodeData->nodeName, "IDLIST") == 0)
                {
                    printf("\n this is an %s\n", children->nodeData->nodeName);
                    pushID(children, sta);
                    printf("\n %s DONE \n", children->nodeData->nodeName);
                }
                else if (strcmp(children->nodeData->nodeName, "DATATYPE") == 0)
                {
                    data.field2 = primitive;
                    data.field3 = malloc(strlen("not_applicable") + 1);
                    data.field3 = "not_applicable";
                    printf("\n this is a DATATYPE with field3 %s\n", data.field3);
                }
                else if (strcmp(children->nodeData->nodeName, "array") == 0)
                {
                    printf("\n this is an %s\n", children->nodeData->nodeName);
                    data.field2 = rectangular;
                    // children = children->next; /////here we are at Dimensions
                }
                else if (strcmp(children->nodeData->nodeName, "DIMENSIONS") == 0)
                {
                    printf("\n this is a %s node \n", children->nodeData->nodeName);
                    findDimensions(children);
                    printf("\n findDimensions done for %s\n", children->nodeData->nodeName);
                    int dim = children->nodeData->subExpression->dimensions.count;
                    bool dynamic = false;
                    for (int i = 0; i < dim; i++)
                    {
                        char *r1 = children->nodeData->subExpression->dimensions.ranges[i][0];
                        char *r2 = children->nodeData->subExpression->dimensions.ranges[i][1];
                        printf("\nrange %d is [%s .. %s]\n",i+1,r1,r2);
                        if (isIdentifier(r1) || isIdentifier(r2))
                        {
                            data.field3 = malloc(strlen("dynamic")+1);
                            data.field3 = "dynamic";
                            dynamic = true;
                            break;
                        }
                    }
                    if (!dynamic) {
                        data.field3 = malloc(strlen("static")+1);
                        data.field3 = "static";
                    }
                    printf("\nthis array is %s\n",data.field3);
                }
                else if (strcmp(children->nodeData->nodeName, "DIMJAGGED") == 0)
                {
                    printf("\n this is a dimJagged node %s\n", children->nodeData->nodeName);
                    findDimJagged(children);
                }
                else if (strcmp(children->nodeData->nodeName, "MAKEROWS") == 0)
                {
                    printf("\n this is a makerows node %s\n", children->nodeData->nodeName);
                    makeRows(children);
                }

                children = children->next;
                // if(children->next) {
                //     printf("\n at 79 children has become %s\n", children->nodeData->nodeName);
                // }
            }
            printf("exited at 100");
           
            // col = children;

            ////////type expression data for primitive type

            // ////////type expression data for primitive type
            
            // else if (strcmp(col->nodeData->nodeName, "jagged") == 0)
            // {
            //     data.field2 = jagged;
            //     data.field3 = "not_applicable";
            // }

            // int tag = populateTag(data.field2, data.field3);

            // if (tag == 0)
            // {
            //     data.field4.typePrimitive = col->child->nodeData->nodeName;
            // }
            // else if (tag == 1 || tag == 2)
            // {
            //     data.field4.rect.type = "rectangularArray";
            //     data.field4.rect.basicElementType = "integer";
            //     data.field4.rect.dim = col->nodeData->subExpression->dimensions.count;
            //     for (int i = 0; i < data.field4.rect.dim; i++)
            //     {
            //         data.field4.rect.R[i][0] = col->nodeData->subExpression->dimensions.ranges[i][0];
            //         data.field4.rect.R[i][1] = col->nodeData->subExpression->dimensions.ranges[i][1];
            //     }
            // }
            // else if (tag == 2)
            // {
            //     data.field4.jag.basicElementType = "integer";
            //     data.field4.jag.type = "jaggedArray";
            //     col = col->next->next;
            //     data.field4.jag.dim = col->nodeData->subExpression->dimJagged.dim;
            //     data.field4.jag.R1[0] = col->nodeData->subExpression->dimJagged.num1;
            //     data.field4.jag.R1[1] = col->nodeData->subExpression->dimJagged.num2;
            //     while (strcmp(col->nodeData->nodeName, "MAKEROWS") != 0)
            //     {
            //         col = col->next;
            //     }
            //     if (col->nodeData->subExpression->makerows.is3D == 0 && data.field4.jag.dim == 3 || col->nodeData->subExpression->makerows.is3D == 1 && data.field4.jag.dim == 2)
            //     {

            //         //////////////////print error for dimension mismatch
            //     }
            //     if (col->nodeData->subExpression->makerows.is3D == 0)
            //     {
            //         int rows = col->nodeData->subExpression->makerows.rows;
            //         data.field4.jag._2dor3d.r2 = (int *)malloc(sizeof(int) * rows);
            //         for (int i = 0; i < rows; i++)
            //         {
            //             data.field4.jag._2dor3d.r2[i] = (int) atoi(col->nodeData->subExpression->makerows.size[i]);
            //         }

            //         ///////////at thids point jagged data structure for d is complete.
            //         /////////error checking left
            //         for (int i = 0; i < rows; i++)
            //         {
            //             if (data.field4.jag._2dor3d.r2[i] != col->nodeData->subExpression->makerows.listselect.Numlist[i].count)
            //             {
            //                 ///////////////////////print size mismatch error for 2d
            //             }
            //         }
            //     }
            //     else if (col->nodeData->subExpression->makerows.is3D == 1)
            //     {
            //         int rows = col->nodeData->subExpression->makerows.rows;
            //         data.field4.jag._2dor3d.r2_ = (jagged3d *)malloc(sizeof(int) * rows);
            //         for (int i = 0; i < rows; i++)
            //         {
            //             data.field4.jag._2dor3d.r2_[i].r = (int) atoi(col->nodeData->subExpression->makerows.size[i]);
            //             int size = data.field4.jag._2dor3d.r2_[i].r;
            //             if (size != col->nodeData->subExpression->makerows.listselect.Listofnumlist->count)
            //             {

            //                 ////////////row size mismatch error print
            //             }
            //             int s = col->nodeData->subExpression->makerows.listselect.Listofnumlist->count;
            //             data.field4.jag._2dor3d.r2_[i].arr = (int *)malloc(sizeof(int) * s);
            //             for (int j = 0; j < s; j++)
            //             {
            //                 data.field4.jag._2dor3d.r2_[i].arr[j] = col->nodeData->subExpression->makerows.listselect.Listofnumlist[i].listofnums[j].count;
            //             }
            //         }
            //     }
            //     int range1 = (int) atoi(data.field4.jag.R1[0]);
            //     int range2 = (int) atoi(data.field4.jag.R1[1]);
            //     if (range2 <= range1)
            //     {

            //         /////////////////print range error where H <= L
            //     }
            //     if (abs(range2 - range1 + 1) != col->nodeData->subExpression->makerows.rows)
            //     {

            //         ///////////////print number of rows mismatch error
            //     }
            //     for (int k = 0; k < col->nodeData->subExpression->makerows.rows; k++)
            //     {
            //         int rowNo = (int) atoi(col->nodeData->subExpression->makerows.listofrows[k]);
            //         if (rowNo < range1 || rowNo > range2)
            //         {

            //             /////////print rowNo error
            //         }
            //     }
            // }
            while (!newIsEmpty(sta))
            {
                parseTreeNode *var = sta->top->node;
                // if(!var) {
                //     printf("var is NULL");
                // }
                // printf("\nnow pushing ID %s\n",var->nodeData->nodeName);
                newPop(sta);
                // printf("\nnow adding type\n");
                data.field1 = malloc(strlen(var->nodeData->nodeName)+1);
                strcpy(data.field1, var->nodeData->nodeName);
                ///////////////call for a=expression table insertion
                addType(data,T);
            }
            inOrder(n->next, T);
        }

        else if(strcmp(n->nodeData->nodeName,"ASSIGNMENTSTATEMENT")==0){


                // a=a+b

            



        }
        else if (!isTerminal(n->nodeData->nodeName))
        {
            inOrder(n->child, T);
            inOrder(n->next, T);
        }
        else
            n = n->next;
    }
}
// R1 [ 4 ] : size 3 : values { 21 641 23 36 125 ; 54 221 43 ; 287 501 453 334 23 }
/* MAKEROWS R1 SQUAREOP static_const SQUARECL COLON size static_const COLON values CURLYOP NUMLIST CURLYCL MAKEROWS2
//MAKEROWS R1 SQUAREOP static_const SQUARECL COLON size static_const COLON values CURLYOP LISTOFNUMLIST CURLYCL MAKEROWS2 */
/* MAKEROWS2 MAKEROWS
MAKEROWS2 epsilon */

//makerows2 makerows
//makerows2 epsilon
void makeRows(parseTreeNode *node)
{
    if (!node)
        return;
    if (!node->child)
        return;

    parseTreeNode *temp = node->child;
    char *rowno;
    char *sizevar;
    int count1 = 0;
    struct NUMLIST nl;
    struct LISTOFNUMLIST lnl;

    while (temp)
    {
        if (strcmp(temp->nodeData->nodeName, "static_const") == 0)
        {
            rowno = temp->nodeData->nodeName;
            //temp->nodeData->subExpression->terminal.type = temp->nodeData->nodeName;
        }
        else if (strcmp(temp->nodeData->nodeName, "size") == 0)
        {
            temp = temp->next;
            sizevar = temp->nodeData->nodeName;
        }
        else if (strcmp(temp->nodeData->nodeName, "NUMLIST") == 0)
        {
            node->nodeData->subExpression->makerows.is3D = 0;
            findNumlist(temp);
            nl = temp->nodeData->subExpression->numlist;
        }
        else if (strcmp(temp->nodeData->nodeName, "LISTOFNUMLIST") == 0)
        {
            node->nodeData->subExpression->makerows.is3D = 1;
            findListofNumlist(temp);
            lnl = temp->nodeData->subExpression->listofnumlist;
        }
        else if (strcmp(temp->nodeData->nodeName, "MAKEROWS2") == 0)
        {
            if (!temp->child)
            {
                node->nodeData->subExpression->makerows.rows = 1;
                node->nodeData->subExpression->makerows.listofrows = (char **)malloc(sizeof(char *));
                node->nodeData->subExpression->makerows.size = (char **)malloc(sizeof(char *));
                node->nodeData->subExpression->makerows.listofrows[0] = (char *)malloc(strlen(rowno) + 1);
                node->nodeData->subExpression->makerows.listofrows[0] = rowno;
                node->nodeData->subExpression->makerows.size[0] = (char *)malloc(strlen(sizevar) + 1);
                node->nodeData->subExpression->makerows.size[0] = sizevar;
                if (node->nodeData->subExpression->makerows.is3D == 0)
                {
                    node->nodeData->subExpression->makerows.listselect.Numlist = (NUMLIST *)malloc(sizeof(NUMLIST));
                    node->nodeData->subExpression->makerows.listselect.Numlist[0] = nl;
                }
                else
                {
                    node->nodeData->subExpression->makerows.listselect.Listofnumlist = (LISTOFNUMLIST *)malloc(sizeof(LISTOFNUMLIST));
                    node->nodeData->subExpression->makerows.listselect.Listofnumlist[0] = lnl;
                }
                return;
            }
            else
            {
                makeRows(temp->child);
                node->nodeData->subExpression->makerows.rows = 1 + temp->child->nodeData->subExpression->makerows.rows;
                node->nodeData->subExpression->makerows.listofrows = (char **)malloc((temp->child->nodeData->subExpression->makerows.rows + 1) * sizeof(char *));
                node->nodeData->subExpression->makerows.size = (char **)malloc((temp->child->nodeData->subExpression->makerows.rows + 1) * sizeof(char *));
                node->nodeData->subExpression->makerows.listofrows[0] = (char *)malloc(strlen(rowno) + 1);
                node->nodeData->subExpression->makerows.listofrows[0] = rowno;
                node->nodeData->subExpression->makerows.size[0] = (char *)malloc(strlen(sizevar) + 1);
                node->nodeData->subExpression->makerows.size[0] = sizevar;
                for (int i = 1, j = 0; i < node->nodeData->subExpression->makerows.rows && j < temp->child->nodeData->subExpression->makerows.rows; i++, j++)
                {
                    node->nodeData->subExpression->makerows.listofrows[i] = (char *)malloc(strlen(temp->child->nodeData->subExpression->makerows.listofrows[j]) + 1);
                    node->nodeData->subExpression->makerows.listofrows[i] = temp->child->nodeData->subExpression->makerows.listofrows[j];
                    node->nodeData->subExpression->makerows.size[i] = (char *)malloc(strlen(temp->child->nodeData->subExpression->makerows.size[j]) + 1);
                    node->nodeData->subExpression->makerows.size[i] = temp->child->nodeData->subExpression->makerows.size[j];
                }
            }
            if (node->nodeData->subExpression->makerows.is3D == 0)
            {
                node->nodeData->subExpression->makerows.listselect.Numlist = (NUMLIST *)malloc(sizeof(NUMLIST) * node->nodeData->subExpression->makerows.rows);
                node->nodeData->subExpression->makerows.listselect.Numlist[0] = nl;
                for (int i = 1, j = 0; i < node->nodeData->subExpression->makerows.rows && j < temp->child->nodeData->subExpression->makerows.rows; i++, j++)
                {
                    node->nodeData->subExpression->makerows.listselect.Numlist[i] = temp->child->nodeData->subExpression->makerows.listselect.Numlist[j];
                }
            }
            else
            {
                node->nodeData->subExpression->makerows.listselect.Listofnumlist = (LISTOFNUMLIST *)malloc(sizeof(LISTOFNUMLIST) * node->nodeData->subExpression->makerows.rows);
                node->nodeData->subExpression->makerows.listselect.Listofnumlist[0] = lnl;
                for (int i = 1, j = 0; i < node->nodeData->subExpression->makerows.rows && j < temp->child->nodeData->subExpression->makerows.rows; i++, j++)
                {
                    node->nodeData->subExpression->makerows.listselect.Listofnumlist[i] = temp->child->nodeData->subExpression->makerows.listselect.Listofnumlist[j];
                }
            }
        }

        temp = temp->next;
    }
}
/* int **arr = (int **)malloc(r * sizeof(int *)); 
    for (i=0; i<r; i++) 
         arr[i] = (int *)malloc(c * sizeof(int)); */

/*NUMLIST static_const semicol NUMLIST 
NUMLIST static_const*/
void findNumlist(parseTreeNode *node)
{
    if (!node)
        return;
    int count1 = 0;
    parseTreeNode *temp = node->child;

    node->nodeData->subExpression->numlist.count++;
    temp->nodeData->subExpression->terminal.type = temp->nodeData->nodeName;
    node->nodeData->subExpression->numlist.listofnum[count1++] = temp->nodeData->nodeName;

    if (temp->next)
    {
        temp = temp->next;
        while (temp)
        {
            if (strcmp(temp->nodeData->nodeName, "semicol") == 0)
            {
                temp->nodeData->subExpression->terminal.type = temp->nodeData->nodeName;
            }
            else
            {
                findNumlist(temp);
                node->nodeData->subExpression->numlist.count += temp->nodeData->subExpression->numlist.count;
                for (int i = count1, j = 0; i < node->nodeData->subExpression->numlist.count && j < temp->nodeData->subExpression->numlist.count; i++, j++)
                {
                    node->nodeData->subExpression->numlist.listofnum[i] = temp->nodeData->subExpression->numlist.listofnum[j];
                }
            }
            temp = temp->next;
        }
    }
}

/* LISTOFNUMLIST NUMS SEMICOL LISTOFNUMLIST 
LISTOFNUMLIST NUMS */
void findListofNumlist(parseTreeNode *node)
{
    if (!node)
        return;
    int count1 = 0;
    parseTreeNode *temp = node->child;

    node->nodeData->subExpression->listofnumlist.count++;
    findNums(temp);
    node->nodeData->subExpression->listofnumlist.listofnums[count1++] = temp->nodeData->subExpression->nums;

    if (temp->next)
    {
        temp = temp->next;
        while (temp)
        {
            if (strcmp(temp->nodeData->nodeName, "semicol") == 0)
            {
                temp->nodeData->subExpression->terminal.type = temp->nodeData->nodeName;
            }
            else
            {
                findListofNumlist(temp);
                node->nodeData->subExpression->listofnumlist.count += temp->nodeData->subExpression->listofnumlist.count;
                for (int i = count1, j = 0; i < node->nodeData->subExpression->listofnumlist.count && j < temp->nodeData->subExpression->listofnumlist.count; i++, j++)
                {
                    node->nodeData->subExpression->listofnumlist.listofnums[i] = temp->nodeData->subExpression->listofnumlist.listofnums[j];
                }
            }
            temp = temp->next;
        }
    }
}

/*NUMS static_const NUMS
NUMS static_const */
void findNums(parseTreeNode *node)
{
    if (!node)
        return;
    int count1 = 0;
    parseTreeNode *temp = node->child;

    node->nodeData->subExpression->nums.count++;
    temp->nodeData->subExpression->terminal.type = temp->nodeData->nodeName;
    node->nodeData->subExpression->nums.num[count1++] = temp->nodeData->nodeName;

    if (temp->next)
    {
        temp = temp->next;
        findNums(temp);
        node->nodeData->subExpression->nums.count += temp->nodeData->subExpression->nums.count;
        for (int i = count1, j = 0; i < node->nodeData->subExpression->nums.count && j < temp->nodeData->subExpression->nums.count; i++, j++)
        {
            node->nodeData->subExpression->nums.num[i] = temp->nodeData->subExpression->nums.num[j];
        }
    }
}

//////////to find DimJagged
/*
DIMJAGGED SQUAREOP static_const range_dots static_const SQUARECL SQUAREOP SQUARECL
DIMJAGGED SQUAREOP static_const range_dots static_const SQUARECL SQUAREOP SQUARECL SQUAREOP SQUARECL
*/
void findDimJagged(parseTreeNode *node)
{
    parseTreeNode *temp = node->child;
    while (temp)
    {
        if (strcmp(temp->nodeData->nodeName, "SQUAREOP") == 0)
        {
            node->nodeData->subExpression->dimJagged.dim++;
            temp->nodeData->subExpression->terminal.type = temp->nodeData->nodeName;
        }
        else if (strcmp(getTokenName(temp->nodeData->nodeName), "static_const") == 0)
        {
            temp->nodeData->subExpression->terminal.type = temp->nodeData->nodeName;
            if (strlen(node->nodeData->subExpression->dimJagged.num1) == 0)
                node->nodeData->subExpression->dimJagged.num1 = temp->nodeData->nodeName;
            else
                node->nodeData->subExpression->dimJagged.num2 = temp->nodeData->nodeName;
        }
        else
        {
            temp->nodeData->subExpression->terminal.type = temp->nodeData->nodeName;
        }
        temp = temp->next;
    }
}

//////////to find dimension of rectangular array
void findDimension(parseTreeNode *p)
{
    parseTreeNode *temp = p->child;
    char *num1;
    char *num2;
    while (temp)
    {
        if (isTerminal(temp->nodeData->nodeName))
        {
            printf("\n terminal is %s\n",temp->nodeData->nodeName);
            temp->nodeData->subExpression->terminal.type = temp->nodeData->nodeName;
            printf("\n terminal copy check %s\n",temp->nodeData->subExpression->terminal.type);
        }
        else
        {
            parseTreeNode *temp2 = temp;
            temp2 = temp2->child;
            struct RANGE r;
            r.num1 = temp2->nodeData->nodeName;
            num1 = r.num1;
            temp2->nodeData->subExpression->terminal.type = temp2->nodeData->nodeName;
            temp2 = temp2->next;
            temp2->nodeData->subExpression->terminal.type = temp2->nodeData->nodeName;
            temp2 = temp2->next;
            r.num2 = temp2->nodeData->nodeName;
            num2 = r.num2;
            temp2->nodeData->subExpression->terminal.type = temp2->nodeData->nodeName;
            temp->nodeData->subExpression->range = r;
        }
        temp = temp->next;
    }
    p->nodeData->subExpression->dimension.range[0] = num1;
    p->nodeData->subExpression->dimension.range[1] = num2;
    printf("\nrange for this dimesion is [%s .. %s]\n",num1,num2);
}

/////////find dimensions remaining
/*
DIMENSIONS DIMENSION DIMENSIONS_REMAINING
DIMENSION SQUAREOP RANGE SQUARECL 
DIMENSIONS_REMAINING DIMENSION DIMENSIONS_REMAINING ////////////tell change to nikhil
DIMENSIONS_REMAINING epsilon
*/
void findDimRemaining(parseTreeNode *p)
{
    if (!p)
        return;
    ///epsilon case
    if (!p->child)
    {
        p->nodeData->subExpression->dimRemaining.count = 0;
        return;
    }
    findDimension(p->child);
    findDimRemaining(p->child->next);
    parseTreeNode *dimen = p->child;
    parseTreeNode *dimenRem = p->child->next;
    p->nodeData->subExpression->dimRemaining.count = 1 + dimenRem->nodeData->subExpression->dimRemaining.count;
    p->nodeData->subExpression->dimRemaining.ranges[0][0] = dimen->nodeData->subExpression->dimension.range[0];
    p->nodeData->subExpression->dimRemaining.ranges[0][1] = dimen->nodeData->subExpression->dimension.range[1];
    for (int i = 1; i < p->nodeData->subExpression->dimRemaining.count; i++)
    {
        p->nodeData->subExpression->dimRemaining.ranges[i][0] = dimenRem->nodeData->subExpression->dimRemaining.ranges[i - 1][0];
        p->nodeData->subExpression->dimRemaining.ranges[i][1] = dimenRem->nodeData->subExpression->dimRemaining.ranges[i - 1][1];
    }
}

void findDimensions(parseTreeNode *p)
{
    findDimension(p->child);
    findDimRemaining(p->child->next);
    parseTreeNode *dimen = p->child;
    parseTreeNode *dimenRem = p->child->next;
    p->nodeData->subExpression->dimensions.count = 1 + dimenRem->nodeData->subExpression->dimRemaining.count;
    p->nodeData->subExpression->dimensions.ranges[0][0] = dimen->nodeData->subExpression->dimension.range[0];
    p->nodeData->subExpression->dimensions.ranges[0][1] = dimen->nodeData->subExpression->dimension.range[1];
    for (int i = 1; i < p->nodeData->subExpression->dimRemaining.count; i++)
    {
        p->nodeData->subExpression->dimensions.ranges[i][0] = dimenRem->nodeData->subExpression->dimRemaining.ranges[i - 1][0];
        p->nodeData->subExpression->dimensions.ranges[i][1] = dimenRem->nodeData->subExpression->dimRemaining.ranges[i - 1][1];
    }
}
/////to find ids in idlist
void pushID(parseTreeNode *p, newStack *s)
{
    if (!p || !p->child)
    {
        return;
    }
    // printf("\n %s 605  \n", p->nodeData->nodeName);
    parseTreeNode *temp = p->child;
    if (temp && isTerminal(temp->nodeData->nodeName))
    {
        newPush(s, temp);
        // temp->nodeData->subExpression->terminal.type = temp->nodeData->nodeName;
        printf("\n pushed %s \n", temp->nodeData->nodeName);
        pushID(temp->next, s); // assuming first child is identifier
    }
    else
    {
        // printf("\n %s is not a terminal \n", temp->next->nodeData->nodeName);
        pushID(temp->child, s);
        pushID(temp->next, s);
    }
}

void newPush(newStack *st, parseTreeNode *node)
{
    newStackNode *temp = (newStackNode *)malloc(sizeof(newStackNode));
    if (!temp)
    {
        printf("\nHeap Overflow");
        exit(1);
    }
    // printf("\n push begin\n");
    temp->next = NULL;

    if (st->top == NULL)
    {
        st->top = temp;
    }
    else
    {
        temp->next = st->top;
        st->top = temp;
    }
    st->top->node = node;
    st->count++;
    // printf("\n push done\n");

}

void newPop(newStack *st)
{
    if (st->top == NULL)
    {
        printf("Stack already empty!");
        exit(1);
    }
    else
    {
        newStackNode *temp = st->top;
        st->top = st->top->next;
        st->count--;
        free(temp);
        temp = NULL;
        printf("\ndeleted\n");
    }
}
bool newIsEmpty(newStack *st)
{
    return st->top == NULL;
}


// check for subrange
// for (int i = 0; i < dim; i++)
// {
//     char *r1 = children->nodeData->subExpression->dimensions.ranges[i][0];
//     char *r2 = children->nodeData->subExpression->dimensions.ranges[i][1];
//     if (isInteger(r1) && isInteger(r2))
//     {
//         int r11 = (int) atoi(r1);
//         int r22 = (int) atoi(r2);
//         if (r11 >= r22)
//         {

//             ////////////////////////////print error
//         }
//     }
// }