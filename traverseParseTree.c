#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parseTree.h"

void traverseParseTree(parseTree *t, typeExpressionTable *T)
{
    if (t->root != NULL)
    {
        inOrder(t->root->child->next->next->next->next, T);
        printf("\nMoving on to assignment statements\n");
        inOrder(t->root->child->next->next->next->next->child->next, T);
    }
}

void inOrder(parseTreeNode *n, typeExpressionTable *T)
{
    if (n != NULL)
    {
        // printf("in moreassngstmt with child = %s", n->nodeData->nodeName);
        if (strcmp(n->nodeData->nodeName, "STATEMENTS") == 0)
        {
            n = n->child;
        }
        if (strcmp(n->nodeData->nodeName, "DECLARATIONSTATEMENTS") == 0)
        {
            n = n->child;
        }
        if (strcmp(n->nodeData->nodeName, "MOREDECLARATIONSTATEMENTS") == 0)
        {
            if (n->child)
                n = n->child;
        }
        if (strcmp(n->nodeData->nodeName, "MOREASSIGNMENTSTATEMENTS") == 0)
        {
            // printf("in moreassngstmt with child = %s", n->child->nodeData->nodeName);
            if (n->child)
                n = n->child;
        }
        if (n && strcmp(n->nodeData->nodeName, "DECLARATIONSTATEMENT") == 0)
        {
            parseTreeNode *children = n->child;
            parseTreeNode *col = NULL;
            newStack *sta = (newStack *)calloc(1, sizeof(newStack));
            tableData data;
            data.isWrong = false;
            data.lineNo = 0;
            while (children)
            {
                if (isTerminal(children->nodeData->nodeName))
                {
                    if (strcmp(getTokenName(children->nodeData->nodeName), "ID") == 0)
                    {
                        newPush(sta, children);
                    }
                    // else if (strcmp(children->nodeData->nodeName, ":") == 0)
                    // {

                    // }
                    // else if (strcmp(children->nodeData->nodeName, ";") == 0)
                    // {
                    // }
                    else if (strcmp(children->nodeData->nodeName, "array") == 0)
                    {
                        data.field2 = rectangular;
                    }
                    children->nodeData->subExpression->terminal.type = children->nodeData->nodeName;
                }

                //IDLIST
                else if (strcmp(children->nodeData->nodeName, "IDLIST") == 0)
                {
                    pushID(children, sta);
                }
                else if (strcmp(children->nodeData->nodeName, "DATATYPE") == 0)
                {
                    col = children;
                    data.field2 = primitive;
                    data.lineNo = children->nodeData->lineNo;
                    data.field3 = calloc(1, strlen("not_applicable") + 1);
                    data.field3 = "not_applicable";
                }
                else if (strcmp(children->nodeData->nodeName, "DIMENSIONS") == 0)
                {
                    col = children;
                    data.lineNo = children->nodeData->lineNo;
                    findDimensions(children);
                    int dim = children->nodeData->subExpression->dimensions.count;
                    bool dynamic = false;
                    for (int i = 0; i < dim; i++)
                    {
                        char *r1 = children->nodeData->subExpression->dimensions.ranges[i][0];
                        char *r2 = children->nodeData->subExpression->dimensions.ranges[i][1];
                        if (isIdentifier(r1) || isIdentifier(r2))
                        {
                            data.field3 = calloc(1, strlen("dynamic") + 1);
                            data.field3 = "dynamic";
                            dynamic = true;
                            break;
                        }
                    }
                    if (!dynamic)
                    {
                        data.field3 = calloc(1, strlen("static") + 1);
                        data.field3 = "static";
                    }
                }
                else if (strcmp(children->nodeData->nodeName, "DIMJAGGED") == 0)
                {
                    data.field2 = jagged;
                    data.field3 = "not_applicable";
                    data.lineNo = children->nodeData->lineNo;
                    findDimJagged(children);
                    data.field4.jag.dim = children->nodeData->subExpression->dimJagged.dim;
                    col = children;
                }
                else if (strcmp(children->nodeData->nodeName, "MAKEROWS") == 0)
                {
                    findMakeRows(children);
                }

                children = children->next;
            }

            int tag = populateTag(data.field2, data.field3);
            data.tag = tag;
            if (tag == 0)
            {
                data.field4.typePrimitive = col->child->nodeData->nodeName; // DATATYPE node
            }
            else if (tag == 1 || tag == 2)
            {
                data.field4.rect.type = "rectangularArray";
                data.field4.rect.basicElementType = "integer";
                data.field4.rect.dim = col->nodeData->subExpression->dimensions.count; // DIMENSIONS node
                for (int i = 0; i < data.field4.rect.dim; i++)
                {
                    data.field4.rect.R[i][0] = col->nodeData->subExpression->dimensions.ranges[i][0];
                    data.field4.rect.R[i][1] = col->nodeData->subExpression->dimensions.ranges[i][1];
                    printf("assigning %s = %s\n",data.field4.rect.R[i][0],col->nodeData->subExpression->dimensions.ranges[i][0]);
                    printf("assigning %s = %s\n",data.field4.rect.R[i][1],col->nodeData->subExpression->dimensions.ranges[i][1]);
                }
            }
            else if (tag == 3)
            {
                data.field4.jag.basicElementType = "integer";
                data.field4.jag.type = "jaggedArray";
                data.field4.jag.dim = col->nodeData->subExpression->dimJagged.dim;
                data.field4.jag.R1[0] = col->nodeData->subExpression->dimJagged.num1;
                data.field4.jag.R1[1] = col->nodeData->subExpression->dimJagged.num2;
                while (strcmp(col->nodeData->nodeName, "MAKEROWS") != 0)
                {
                    col = col->next;
                }
                if (col->nodeData->subExpression->makerows.is3D == 0 && data.field4.jag.dim == 3 || col->nodeData->subExpression->makerows.is3D == 1 && data.field4.jag.dim == 2)
                {
                    if (data.field4.jag.dim == 2)
                    {
                        data.isWrong = true;
                        printDecErrors(col->nodeData->lineNo - 1, col->depth, "Type definition error for 2D jagged array (WRONG MAKEROWS INIT)", NULL);
                    }
                    else if (data.field4.jag.dim == 3)
                    {
                        data.isWrong = true;
                        printDecErrors(col->nodeData->lineNo - 1, col->depth, "Type definition error for 3D jagged array (WRONG MAKEROWS INIT)", NULL);
                    }
                }
                if (col->nodeData->subExpression->makerows.is3D == 0)
                {
                    int rows = col->nodeData->subExpression->makerows.rows;
                    data.field4.jag._2dor3d.r2 = (int *)calloc(1, sizeof(int) * rows);
                    for (int i = 0; i < rows; i++)
                    {
                        data.field4.jag._2dor3d.r2[i] = (int)atoi(col->nodeData->subExpression->makerows.size[i]);
                        printf("%d ===== %s\n",data.field4.jag._2dor3d.r2[i], col->nodeData->subExpression->makerows.size[i]);
                    }

                    int optionalVar;
                    for (int i = 0; i < rows; i++)
                    {
                        if (data.field4.jag._2dor3d.r2[i] != col->nodeData->subExpression->makerows.listselect.Numlist[i].count)
                        {
                            optionalVar = i + 1;
                            data.isWrong = true;
                            printDecErrors(col->nodeData->lineNo + i, col->depth + i + 7, "2D JA size mismatch at row no", &optionalVar);
                        }
                    }
                }
                else if (col->nodeData->subExpression->makerows.is3D == 1)
                {
                    int rows = col->nodeData->subExpression->makerows.rows;
                    data.field4.jag._2dor3d.r2_ = (jagged3d *)calloc(1, sizeof(int) * rows);
                    int optionalVar = 0;
                    for (int i = 0; i < rows; i++)
                    {
                        data.field4.jag._2dor3d.r2_[i].r = (int)atoi(col->nodeData->subExpression->makerows.size[i]);
                        int size = data.field4.jag._2dor3d.r2_[i].r;
                        if (size != col->nodeData->subExpression->makerows.listselect.Listofnumlist[i].count)
                        {
                            optionalVar = i + 1;
                            data.isWrong = true;
                            printDecErrors(col->nodeData->lineNo + i, col->depth + i + 7, "3D JA size mismatch at row no", &optionalVar);
                        }
                        int s = col->nodeData->subExpression->makerows.listselect.Listofnumlist->count;
                        data.field4.jag._2dor3d.r2_[i].arr = (int *)calloc(1, sizeof(int) * s);
                        for (int j = 0; j < s; j++)
                        {
                            data.field4.jag._2dor3d.r2_[i].arr[j] = col->nodeData->subExpression->makerows.listselect.Listofnumlist[i].listofnums[j].count;
                            printf("%d ========== %d\n",data.field4.jag._2dor3d.r2_[i].arr[j], col->nodeData->subExpression->makerows.listselect.Listofnumlist[i].listofnums[j].count);
                        }
                    }
                }
                int range1 = (int)atoi(data.field4.jag.R1[0]);
                int range2 = (int)atoi(data.field4.jag.R1[1]);
                if (range2 <= range1)
                {
                    data.isWrong = true;
                    printDecErrors(col->nodeData->lineNo - 1, col->depth, "Invalid range (lower bound >= Higher Bound)", NULL);
                }
                if (abs(range2 - range1 + 1) != col->nodeData->subExpression->makerows.rows)
                {
                    data.isWrong = true;
                    printDecErrors(col->nodeData->lineNo - 1, col->depth, "Incorrect number of rows intialized", NULL);
                }
                int optionalVar;
                for (int k = 0; k < col->nodeData->subExpression->makerows.rows && range1 <= range2; k++, range1++)
                {
                    int rowNo = (int)atoi(col->nodeData->subExpression->makerows.listofrows[k]);
                    if (rowNo != range1)
                    {
                        optionalVar = rowNo;
                        data.isWrong = true;
                        printDecErrors(col->nodeData->lineNo + k, col->depth, "Row index is wrong in row with index", &optionalVar);
                    }
                }
            }
            while (!newIsEmpty(sta))
            {
                parseTreeNode *var = sta->top->node;
                newPop(sta);
                data.field1 = calloc(1, strlen(var->nodeData->nodeName) + 1);
                strcpy(data.field1, var->nodeData->nodeName);
                addType(data, T);
            }
            inOrder(n->next, T);
        }

        else if (strcmp(n->nodeData->nodeName, "ASSIGNMENTSTATEMENT") == 0)
        {
            printf("\nwithin assn stmt\n");
            parseTreeNode *temp = n->child;
            if (strcmp(temp->nodeData->nodeName, "ARITHMETICASSIGNMENTSTATEMENT") == 0)
            {
                parseTreeNode *arith = temp->child->child;
                char *var = arith->nodeData->nodeName;
                printf("\nwithin arith ass stmt with first id = %s\n", var);
                tableData *lhs = findvar(arith, T);
                if(!lhs) {
                    printf("\nLHS is wrong\n");
                }
                else {
                    printf("\nfindvar done with LHS = %s\n", lhs->field1);
                    if (lhs->field2 == primitive && strcmp(lhs->field4.typePrimitive, "boolean") == 0)
                    {
                        // printAssnErrors()
                    }
                }
                // if (isArithOperator(arith))
                // {
                //     char *op = getTokenName(arith);
                // }
                arith = temp->child->next->next; //In ARITHEXPR2
                printf("\nnow arith is %s\n", arith->nodeData->nodeName);

                tableData *rhs = recurseArithExp(arith, T);
                if(!rhs) {
                    printf("\nnull\n");
                } else {
                    printf("success in rhs");
                }
                printf("\n\n******FINALLY RETURNED NO RECURSION NOW*****\n\n");
            }

            else if (strcmp(temp->nodeData->nodeName, "LOGICALASSIGNMENTSTATEMENT") == 0)
            {
                printf("\nwithin logic ass stmt\n");
                parseTreeNode *logic = temp->child;
                char *var = logic->nodeData->nodeName;
                tableData *lhs = findType(var, T);
                printf("\ntype of lhs is %s\n", lhs->field4.typePrimitive);
                logic = logic->next->next;
                int flag = 0;
                parseTreeNode *logicExpNode = recurseLogicalExpr(logic, T, &flag);

                if (logicExpNode == NULL)
                {
                    // skip over
                }
                else if (lhs->field2 != primitive || strcmp(lhs->field4.typePrimitive, "boolean") != 0)
                {
                    printAssnErrors(logic, logicExpNode, "=", "Type error: LHS is not a boolean variable");
                }
            }
            if (n->next)
            {
                printf("next is %s", n->next->nodeData->nodeName);
                inOrder(n->next, T);
            }
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

//ARITHEXPR ARITHEXPR2 plus_op ARITHEXPR
// ARITHEXPR ARITHEXPR2 minus_op 0ARITHEXPR
// ARITHEXPR ARITHEXPR2
// ARITHEXPR2 VARNUM mul_op ARITHEXPR2
// ARITHEXPR2 VARNUM div_op ARITHEXPR2
// ARITHEXPR2 VARNUM
// VAR ID
// VAR ID SQUAREOP INDEXES SQUARECL
// VARNUM static_const
// VARNUM VAR
/* INDEXES INDEX INDEXES_REM
INDEXES_REM INDEX INDEXES_REM
INDEXES_REM epsilon
INDEX ID
INDEX static_const
*/

//typedef enum assignType{mismatch,boolean,integer,real,array};

////start with first Arithexpr2
tableData *recurseArithExp(parseTreeNode *p, typeExpressionTable *T)
{
    if (!p)
        return NULL;
    tableData *d = NULL, *t = NULL;

    ////if we are at Arithexpr2
    if (strcmp(p->nodeData->nodeName, "ARITHEXPR2") == 0)
    {
        printf("started with AREXP2\n");
        if (strcmp(p->child->nodeData->nodeName, "VARNUM") == 0)
        {
            printf("now at VARNUM\n");
            parseTreeNode *tmp = p->child->child; // tmp = VAR
            if (strcmp(getTokenName(tmp->nodeData->nodeName), "static_const") == 0)
            {
                printf("found a stat const = %s\n", tmp->nodeData->nodeName);
                p->nodeData->subExpression->data.misMatch = 0;
                p->nodeData->subExpression->data.table.field1 = tmp->nodeData->nodeName;
                p->nodeData->subExpression->data.table.field2 = primitive;
                p->nodeData->subExpression->data.table.field3 = "not_applicable";
                p->nodeData->subExpression->data.table.field4.typePrimitive = "integer";
                d = &p->nodeData->subExpression->data.table;
            }
            else if (strcmp(tmp->nodeData->nodeName, "VAR") == 0)
            {
                d = findvar(tmp->child, T);
                if(!d) {
                    return NULL;
                }
                printf("found var = %s of type %s\n", d->field1, d->field4.typePrimitive);
                p->nodeData->subExpression->data.misMatch = 0; // u[2 3]
                p->nodeData->subExpression->data.table = *d;
                printf("\n%d, %s for %s\n",d->field2,d->field4.typePrimitive,d->field1);
                if (d->field2 == primitive && strcmp(d->field4.typePrimitive, "boolean") == 0)
                {
                    printf("found an error type of %s is boolean\n", d->field1);
                    //////////print type error
                    return NULL;
                }
            }
            if (p->child->next && p->child->next->next)

            { //////////////////recursion done for arithexpr2
                printf("at arithexpr calling for %s \n",p->child->next->next->nodeData->nodeName);
                t = recurseArithExp(p->child->next->next, T); // ARITHEXPR2
                if(t == NULL) {
                    return NULL;
                }
                printf("returned");
                p->child->next->next->nodeData->subExpression->data.table = *t;
                p->child->next->next->nodeData->subExpression->data.misMatch = 0;
                printf("\ncalling compare with %s == %s\n",p->child->next->next->child->child->child->nodeData->nodeName,p->child->child->child->nodeData->nodeName); // tmp = VAR, p = ARITHEXPR2
                bool compare = compareSubExpr(p->child->next->next->child->child->child, p->child->child->child, T);
                // 	new = d * e2 + w - 76875 + d ;
                if (compare)
                {
                    //p->nodeData->subExpression->data.table.field1=findVarname();
                    ///no error
                    if(strcmp(p->child->next->nodeData->nodeName,"/") == 0) {
                        if(d->tag == 0) {
                            d->field4.typePrimitive = "real";
                            p->nodeData->subExpression->data.table.field4.typePrimitive = "real";
                        } else if(d->tag == 1 || d->tag == 2) {
                            d->field4.rect.basicElementType = "real";
                            p->nodeData->subExpression->data.table.field4.rect.basicElementType = "real";
                        } else if(d->tag == 3) {
                            d->field4.jag.basicElementType = "real";
                            p->nodeData->subExpression->data.table.field4.jag.basicElementType = "real";
                        }
                        printf("tags were %d and %d\n",t->tag,d->tag);
                        if(t->tag == 0) {
                            t->field4.typePrimitive = "real";
                            p->child->next->next->nodeData->subExpression->data.table.field4.typePrimitive = "real";
                        } else if(t->tag == 1 || t->tag == 2) {
                            t->field4.rect.basicElementType = "real";
                            p->child->next->next->nodeData->subExpression->data.table.field4.rect.basicElementType = "real";
                        } else if(t->tag == 3) {
                            t->field4.jag.basicElementType = "real";
                            p->child->next->next->nodeData->subExpression->data.table.field4.jag.basicElementType = "real";
                        }
                        // p->child->next->next->nodeData->subExpression->data.table = *t;
                        // p->child->next->next->nodeData->subExpression->data.table.field4.typePrimitive = "real";
                        // p->nodeData->subExpression->data.table.field4.typePrimitive = "real";
                        printf("set real for %s and %s\n",d->field1,p->child->next->next->nodeData->subExpression->data.table.field4.typePrimitive);
                    }
                    printf("compare success\n");
                }
                else
                {
                    printf("compare was false");
                    p->nodeData->subExpression->data.misMatch = 1;

                    ////////////print type mismatch error

                    return NULL;
                }
            }
            if(p->next) {
                    tableData *t1 = recurseArithExp(p->next->next, T); // ARITHEXPR
                    if(t1 == NULL) return NULL;
                    p->next->next->nodeData->subExpression->data.table = *t1;
                    p->next->next->nodeData->subExpression->data.misMatch = 0;

                    if(t) {
                        bool compare = compareSubExpr(p->next->next, p->child->next->next, T);
                        if(compare) {
                            printf("\nat 406 compare bw arithExpr2 and arithExpr is true\n");
                            return t1;
                        } else {
                            printf("\nARITHEXPR2 was not expandable\n");
                            return NULL;
                        }
                    } else {
                        bool compare = compareSubExpr(p->next->next, p, T);
                        if(compare) {
                            printf("\nat 415 compare bw arithExpr2 and arithExpr is true\n");
                            return t1;
                        } else {
                            printf("\nARITHEXPR2 was not expandable\n");
                            return NULL;
                        }
                    }
            }
            else {
                printf("\n\nreturning to prev call ARITHEXPR2 ends here\n\n");
                return d;
            }
        }
    }
    if (strcmp(p->nodeData->nodeName, "ARITHEXPR") == 0)
    {
        printf("\ninside arith, expor with child = %s\n",p->child->nodeData->nodeName);
        d = recurseArithExp(p->child, T); // for ARITHEXPR2
        if (d == NULL)
            return NULL; //////if we have already found an error
        parseTreeNode *tmp = p->child; // ARITHEXPR2
        tmp->nodeData->subExpression->data.table = *d;
        tmp->nodeData->subExpression->data.misMatch = 0;
        printf("done with recursion at line 397 %s found\n", d->field1);
        return d;
        // 	new = d * e2 + w - 76875 + d ;
        // if (tmp->next != NULL)
        // {
            /////recurse for arithmaticexpr2
            // tableData *t = recurseArithExp(tmp->next->next, T); // ARITHEXPR
            // tmp->next->next->nodeData->subExpression->data.table = *t;
            // tmp->next->next->nodeData->subExpression->data.misMatch = 0;
            // printf("calling compare with\n");
            // bool compare = true;
            // bool compare = compareSubExpr(tmp, tmp->next->next);
            // if (compare)
            // {
                //p->nodeData->subExpression->data.table.field1=findVarname();
                ///no error
                // tmp = tmp->next->next;
                // if(tmp->child->next) {
                //     printf("compare was true in arexpr, so calling next on %s\n",tmp->child->next->nodeData->nodeName);
                //     printf("inside tmp child next\n");
                //     tableData *t1 = recurseArithExp(tmp->child->next->next, T);
                //     tmp->next->next->next->nodeData->subExpression->data.table = *t1;
                //     tmp->next->next->next->nodeData->subExpression->data.misMatch = 0;
                // } else {
                //     printf("\nNo more recursion this ends here\n");
                // }
                // return d;
            // }
        //     else
        //     {
        //         p->nodeData->subExpression->data.misMatch = 1;

        //         ////////////print type mismatch error

        //         return NULL;
        //     }
        // }
        // else {
        //     printf("returning to previous call, this has no next\n");
        //     return d;
        // }
    }
}
bool compareSubExpr(parseTreeNode *p1, parseTreeNode *p2, typeExpressionTable *T)
{
    tableData *t1 = &p1->nodeData->subExpression->data.table;
    tableData *t2 = &p2->nodeData->subExpression->data.table;
    if (t1->field2 == primitive && t2->field2 != primitive || t1->field2 != primitive && t2->field2 == primitive)
        return false;
    if (t1->field2 == primitive && t2->field2 == primitive)
    {
        printf("both are primitive, %s is %s and %s is %s\n",t1->field1,t1->field4.typePrimitive,t2->field1,t2->field4.typePrimitive);
        if (strcmp(t1->field4.typePrimitive, "integer") && strcmp(t2->field4.typePrimitive, "integer"))
            return true;
        if (strcmp(t1->field4.typePrimitive, "real") && strcmp(t2->field4.typePrimitive, "real"))
            return true;
        return false;
    }
    /////////////////////////rectangular and jagged comparision left
    if (t1->field2 == rectangular && t2->field2 == rectangular)
    {
        if (strcmp(t1->field3, "static") && strcmp(t2->field3, "static"))
        {
            if (t1->field4.rect.dim == t2->field4.rect.dim)
            {
                for (int i = 0; i < t1->field4.rect.dim; i++)
                {
                    if (strcmp(t1->field4.rect.R[i][0], t2->field4.rect.R[i][0]) != 0 || strcmp(t1->field4.rect.R[i][1], t2->field4.rect.R[i][1]) != 0)
                        return false;
                }
                return true;
            }
            else
                return false;
        }
        else if (strcmp(t1->field3, "dynamic") && strcmp(t2->field3, "dynamic"))
        {
            if (t1->field4.rect.dim == t2->field4.rect.dim)
            {
                for (int i = 0; i < t1->field4.rect.dim; i++)
                {
                    if (strcmp(t1->field4.rect.R[i][0], t2->field4.rect.R[i][0]) != 0 || strcmp(t1->field4.rect.R[i][1], t2->field4.rect.R[i][1]) != 0)
                        return false;
                }
                return true;
            }
            else
                return false;
        }
        else
            return false;
    }
    if (t1->field2 == rectangular && t2->field2 != rectangular || t1->field2 != rectangular && t2->field2 == rectangular)
        return false;
    if (p1->nodeData->subExpression->data.table.lineNo != p2->nodeData->subExpression->data.table.lineNo)
        return false;
    return true;
}

tableData *findvar(parseTreeNode *p, typeExpressionTable *T)
{
    tableData *d = findType(p->nodeData->nodeName, T); //ID
    p->nodeData->subExpression->data.table = *d;
    if (d->isWrong)
    {
        //printf() error
        printf("printf iswrong");
        return NULL;
    }
    if (!p->next)
    {
        printf("\nthis was just an ID so no next\n");
        return d;
    }
    else
    {
        printf("\n%s is not primitive\n", p->nodeData->nodeName);
        bool r;
        ////we are sending child of indexes thst is we are at index
        if (d->field2 == rectangular)
            r = traverseIndexesRectangular(p->next->next->child, *d, 0);
        else if (d->field2 == jagged)
        {
            printf("%d are the dim of this jag",d->field4.jag.dim);
            if (d->field4.jag.dim == 2)
                r = traverseIndexes2djagged(p->next->next->child, *d, 0, 0, T);
            else
                r = traverseIndexes3djagged(p->next->next->child, *d, 0, 0, 0, T);
        }
        if (r == false)
        {
            printf("\nreturning false\n");
            printf("\nreturning false\n");
            printf("\nreturning false\n");
            return NULL;
        }
    }
    printf("\noutside of else at 481\n");
    d->field2 = primitive;
    d->field3 = "not_applicable";
    d->tag = 0;
    d->field4.typePrimitive = "integer";
    return d;
}
//////starting at id of var id squareop indexes squarecl
char *findVarname(parseTreeNode *p)
{
    if (!p)
        return NULL;
    char *name;
    if (isTerminal(p->nodeData->nodeName))
    {
        name = p->nodeData->nodeName;
        if (p->next != NULL)
            strcat(name, findVarname(p->next)); ///
    }
    else
    {
        name = findVarname(p->child);
        if (p->next != NULL)
            strcat(name, findVarname(p->next));
    }
    return name;
}
/*  INDEXES INDEX INDEXES_REM
    INDEXES_REM INDEX INDEXES_REM
        INDEXES_REM epsilon
        INDEX ID
        INDEX static_const
        */
bool traverseIndexesRectangular(parseTreeNode *p, tableData d, int dim)
{
    if (!p)
    {
        return true;
    }
    if (isIdentifier(p->child->nodeData->nodeName))
    {
        printf("it is identifier = %s\n",p->child->nodeData->nodeName);
        return true;
    }

    if (strcmp(getTokenName(p->child->nodeData->nodeName), "static_const") == 0)
    {

        printf("it is stat _ const = %s\n",p->child->nodeData->nodeName);
        if (strcmp(d.field3, "static") == 0)
        {
            printf("inside static block\n");
            int r1 = atoi(d.field4.rect.R[dim][0]);
            int r2 = atoi(d.field4.rect.R[dim][1]);
            if (!(atoi(p->child->nodeData->nodeName) >= r1 && atoi(p->child->nodeData->nodeName) <= r2))
            {
                printf("\nARRAY OUT OF BOUNDSSSS\n");
                //error array outof bounds
                return false;
            }
            else
                return traverseIndexesRectangular(p->next->child, d, dim + 1);
        }
        else
        {
            bool b1 = isIdentifier(d.field4.rect.R[dim][0]), b2 = isIdentifier(d.field4.rect.R[dim][1]);
            if (b1 && b2)
            {
                return traverseIndexesRectangular(p->next->child, d, dim + 1);
            }
            else if (b1 && !b2)
            {
                if (atoi(p->child->nodeData->nodeName) > atoi(d.field4.rect.R[dim][1]))
                {
                    printf("2d dim out of bound\n");
                    //error array outof bounds
                    return false;
                }
                else
                    return traverseIndexesRectangular(p->next->child, d, dim + 1);
            }
            else if (!b1 && b2)
            {
                if (atoi(p->child->nodeData->nodeName) < atoi(d.field4.rect.R[dim][0]))
                {
                    printf("2d dim out of bound\n");
                    //error array outof bounds
                    return false;
                }
                else
                    return traverseIndexesRectangular(p->next->child, d, dim + 1);
            }
        }
    }
}

bool traverseIndexes2djagged(parseTreeNode *p, tableData d, int dim, int dimval, typeExpressionTable *T)
{
    if (isIdentifier(p->child->nodeData->nodeName))
    {
        tableData *temp = findType(p->child->nodeData->nodeName, T);
        if (!(temp->field2 == primitive && strcmp(temp->field4.typePrimitive, "integer") == 0))
        {

            ///////////////print error for not integer type identifier
            return false;
        }
        else             ///////////////
            return true; //////since if 1st dim, I dont need to check the 2nd dim as I dont know the value of the row number
    }
    else if (strcmp(getTokenName(p->child->nodeData->nodeName), "static_const") == 0)
    {
        if (dim == 0)
        {
            int r1 = atoi(d.field4.jag.R1[0]);
            int r2 = atoi(d.field4.jag.R1[1]);
            if (!(atoi(p->child->nodeData->nodeName) >= r1 && atoi(p->child->nodeData->nodeName) <= r2))
            {
                /////////print 1st dimension error
                printf("\nARRAY OUT OF BOUNDS\n");
                return false;
            }
            else
                return traverseIndexes2djagged(p->next->child, d, 1, atoi(p->child->nodeData->nodeName) - r1, T);
        }
        if (dim == 1)
        {
            printf("dimval is %d\n",dimval);
            int size = d.field4.jag._2dor3d.r2[dimval];
            if (atoi(p->child->nodeData->nodeName) < size)
                return true;
            else
            {   
                printf("%d == %d",atoi(p->child->nodeData->nodeName),size);
                printf("\nARRAY OUT OF BOUNDS 2\n");
                ////////////////print error for array size mismatch
                return false;
            }
        }
    }
}
bool traverseIndexes3djagged(parseTreeNode *p, tableData d, int dim, int dim1val, int dim2val, typeExpressionTable *T)
{
    if (isIdentifier(p->child->nodeData->nodeName))
    {
        tableData *temp = findType(p->child->nodeData->nodeName, T);
        if (!(temp->field2 == primitive && strcmp(temp->field4.typePrimitive, "integer") == 0))
        {

            ///////////////print error for not integer type identifier
            return false;
        }
        else             ///////////////
            return true; //////same reason as 2d jagged
    }
    else if (strcmp(getTokenName(p->child->nodeData->nodeName), "static_const") == 0)
    {
        if (dim == 0)
        {
            int r1 = atoi(d.field4.jag.R1[0]);
            int r2 = atoi(d.field4.jag.R1[1]);
            if (!(atoi(p->child->nodeData->nodeName) >= r1 && atoi(p->child->nodeData->nodeName) <= r2))
            {
                /////////print 1st dimension error
                printf("\n3d 1d wrong\n");
                return false;
            }
            else
                return traverseIndexes3djagged(p->next->child, d, 1, atoi(p->child->nodeData->nodeName) - r1, 0, T);
        }
        if (dim == 1)
        {
            int size = d.field4.jag._2dor3d.r2_[dim1val].r;
            if (atoi(p->child->nodeData->nodeName) < size) {
                printf("%s----\n",p->child->nodeData->nodeName);
                return traverseIndexes3djagged(p->next->child, d, 2, dim1val, atoi(p->child->nodeData->nodeName), T);
            }
            else
            {
                // printf("%d == %d\n",atoi(p->child->nodeData->nodeName), size);
                printf("\n3d 2d wrong\n");
                // print error for array size mismatch
                return false;
            }
        }
        if (dim == 2)
        {
            int dim3val = d.field4.jag._2dor3d.r2_[dim1val].arr[dim2val];
            if (atoi(p->child->nodeData->nodeName) < dim3val) {
                printf("%d+++++%d",(int)atoi(p->child->nodeData->nodeName),dim3val);
                return true;
            }
            else
            {
                printf("\n3d 3d wrong %s == %d\n", p->child->nodeData->nodeName, dim3val);
                ////////////////print error for array size mismatch
                return false;
            }
        }
    }
}

tableData *findType(char *var, typeExpressionTable *T)
{
    for (int i = 0; i < T->count; i++)
    {
        if (strcmp(var, T->dataTypeExpr[i].field1) == 0)
        {   
            return &T->dataTypeExpr[i];
        }
    }
}

//LOGICALASSIGNMENTSTATEMENT ID assign_op LOGICALEXPR semicol
// LOGICALEXPR LOGICALEXPR2 logic_or LOGICALEXPR
// LOGICALEXPR LOGICALEXPR2
// LOGICALEXPR2 ID logic_and LOGICALEXPR2
// LOGICALEXPR2 ID
parseTreeNode *recurseLogicalExpr(parseTreeNode *logic, typeExpressionTable *T, int *flag)
{
    if (*flag == 1)
    {
        return NULL;
    }
    if (isTerminal(logic->nodeData->nodeName))
    {
        return logic;
    }
    parseTreeNode *parent = logic;
    parseTreeNode *p1;
    if (logic->child)
        p1 = recurseLogicalExpr(logic->child, T, flag);
    if (!p1)
        return NULL;
    tableData *d = findType(p1->nodeData->nodeName, T);
    p1->nodeData->subExpression->data.table = *d;
    logic->nodeData->subExpression->data.table = *d;
    if (logic->child->next != NULL)
    {
        parseTreeNode *p2 = recurseLogicalExpr(logic->child->next->next, T, flag);
        if (!p2)
            return NULL;
        tableData *d2 = findType(p2->nodeData->nodeName, T);
        p2->nodeData->subExpression->data.table = *d2;
        if ((d->field2 != primitive || strcmp(d->field4.typePrimitive, "boolean") != 0) || (d2->field2 != primitive || strcmp(d2->field4.typePrimitive, "boolean") != 0))
        {
            *flag = 1;
            if (parent->child->next)
            {
                printAssnErrors(p1, p2, parent->child->next->nodeData->nodeName, "Type error: mismatch between datatypes");
                return NULL;
            }
            else
            {
                printAssnErrors(p1, p2, "|||", "Type error: mismatch between datatypes");
            }
            if (d->field2 != primitive || strcmp(d->field4.typePrimitive, "boolean") != 0)
            {
                return p1;
            }
            else
            {
                if (p2->nodeData->subExpression->data.table.field4.typePrimitive)
                    return p2;
            }
        }
    }
    return p1;
}

parseTreeNode *findNextID(parseTreeNode *node)
{
    if (isIdentifier(node->nodeData->nodeName))
    {
        return node;
    }
    if (isTerminal(node->nodeData->nodeName))
    {
        return findNextID(node->next);
    }
    return findNextID(node->child);
}

// R1 [ 4 ] : size 3 : values { 21 641 23 36 125 ; 54 221 43 ; 287 501 453 334 23 }
/* MAKEROWS R1 SQUAREOP static_const SQUARECL COLON size static_const COLON values CURLYOP NUMLIST CURLYCL MAKEROWS2
//MAKEROWS R1 SQUAREOP static_const SQUARECL COLON size static_const COLON values CURLYOP LISTOFNUMLIST CURLYCL MAKEROWS2 */
/* MAKEROWS2 MAKEROWS
MAKEROWS2 epsilon */

//makerows2 makerows
//makerows2 epsilon
void findMakeRows(parseTreeNode *node)
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
        if (strcmp(getTokenName(temp->nodeData->nodeName), "static_const") == 0)
        {
            rowno = temp->nodeData->nodeName;
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
                node->nodeData->subExpression->makerows.listofrows = (char **)calloc(1, sizeof(char *));
                node->nodeData->subExpression->makerows.size = (char **)calloc(1, sizeof(char *));
                node->nodeData->subExpression->makerows.listofrows[0] = (char *)calloc(1, strlen(rowno) + 1);
                node->nodeData->subExpression->makerows.listofrows[0] = rowno;
                node->nodeData->subExpression->makerows.size[0] = (char *)calloc(1, strlen(sizevar) + 1);
                node->nodeData->subExpression->makerows.size[0] = sizevar;
                if (node->nodeData->subExpression->makerows.is3D == 0)
                {
                    node->nodeData->subExpression->makerows.listselect.Numlist = (NUMLIST *)calloc(1, sizeof(NUMLIST));
                    node->nodeData->subExpression->makerows.listselect.Numlist[0] = nl;
                }
                else
                {
                    node->nodeData->subExpression->makerows.listselect.Listofnumlist = (LISTOFNUMLIST *)calloc(1, sizeof(LISTOFNUMLIST));
                    node->nodeData->subExpression->makerows.listselect.Listofnumlist[0] = lnl;
                }
                return;
            }
            else
            {
                findMakeRows(temp->child);
                node->nodeData->subExpression->makerows.rows = 1 + temp->child->nodeData->subExpression->makerows.rows;
                node->nodeData->subExpression->makerows.listofrows = (char **)calloc(1, (temp->child->nodeData->subExpression->makerows.rows + 1) * sizeof(char *));
                node->nodeData->subExpression->makerows.size = (char **)calloc(1, (temp->child->nodeData->subExpression->makerows.rows + 1) * sizeof(char *));
                node->nodeData->subExpression->makerows.listofrows[0] = (char *)calloc(1, strlen(rowno) + 1);
                node->nodeData->subExpression->makerows.listofrows[0] = rowno;
                node->nodeData->subExpression->makerows.size[0] = (char *)calloc(1, strlen(sizevar) + 1);
                node->nodeData->subExpression->makerows.size[0] = sizevar;
                for (int i = 1, j = 0; i < node->nodeData->subExpression->makerows.rows && j < temp->child->nodeData->subExpression->makerows.rows; i++, j++)
                {
                    node->nodeData->subExpression->makerows.listofrows[i] = (char *)calloc(1, strlen(temp->child->nodeData->subExpression->makerows.listofrows[j]) + 1);
                    node->nodeData->subExpression->makerows.listofrows[i] = temp->child->nodeData->subExpression->makerows.listofrows[j];
                    node->nodeData->subExpression->makerows.size[i] = (char *)calloc(1, strlen(temp->child->nodeData->subExpression->makerows.size[j]) + 1);
                    node->nodeData->subExpression->makerows.size[i] = temp->child->nodeData->subExpression->makerows.size[j];
                }
            }
            if (node->nodeData->subExpression->makerows.is3D == 0)
            {
                node->nodeData->subExpression->makerows.listselect.Numlist = (NUMLIST *)calloc(1, sizeof(NUMLIST) * node->nodeData->subExpression->makerows.rows);
                node->nodeData->subExpression->makerows.listselect.Numlist[0] = nl;
                for (int i = 1, j = 0; i < node->nodeData->subExpression->makerows.rows && j < temp->child->nodeData->subExpression->makerows.rows; i++, j++)
                {
                    node->nodeData->subExpression->makerows.listselect.Numlist[i] = temp->child->nodeData->subExpression->makerows.listselect.Numlist[j];
                }
            }
            else
            {
                node->nodeData->subExpression->makerows.listselect.Listofnumlist = (LISTOFNUMLIST *)calloc(1, sizeof(LISTOFNUMLIST) * node->nodeData->subExpression->makerows.rows);
                node->nodeData->subExpression->makerows.listselect.Listofnumlist[0] = lnl;
                for (int i = 1, j = 0; i < node->nodeData->subExpression->makerows.rows && j < temp->child->nodeData->subExpression->makerows.rows; i++, j++)
                {
                    node->nodeData->subExpression->makerows.listselect.Listofnumlist[i] = temp->child->nodeData->subExpression->makerows.listselect.Listofnumlist[j];
                }
            }
        }
        else
        {
            temp->nodeData->subExpression->terminal.type = temp->nodeData->nodeName;
        }
        temp = temp->next;
    }
}

void findNumlist(parseTreeNode *node)
{
    if (!node && !node->child)
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
            if (strcmp(temp->nodeData->nodeName, ";") == 0)
            {
                temp->nodeData->subExpression->terminal.type = temp->nodeData->nodeName;
            }
            else if (strcmp(temp->child->nodeData->nodeName, ";") == 0)
            {
                printDecErrors(temp->nodeData->lineNo, temp->depth, "2D JA size mismatch : Empty number, two `;` shouldn't be consecutively", NULL);
            }
            else
            {
                //2;3;4;5
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
    if (!node && !node->child)
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
            if (strcmp(temp->nodeData->nodeName, ";") == 0)
            {
                temp->nodeData->subExpression->terminal.type = temp->nodeData->nodeName;
            }
            else if (strcmp(temp->child->nodeData->nodeName, ";") == 0)
            {
                printDecErrors(temp->nodeData->lineNo, temp->depth, "3D JA size mismatch : Empty list of nums, two `;` shouldn't be consecutively", NULL);
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

// to find DimJagged
void findDimJagged(parseTreeNode *node)
{
    parseTreeNode *temp = node->child;
    while (temp)
    {
        if (strcmp(temp->nodeData->nodeName, "[") == 0)
        {
            node->nodeData->subExpression->dimJagged.dim++;
            temp->nodeData->subExpression->terminal.type = temp->nodeData->nodeName;
        }
        else if (strcmp(getTokenName(temp->nodeData->nodeName), "static_const") == 0)
        {
            temp->nodeData->subExpression->terminal.type = temp->nodeData->nodeName;
            if (node->nodeData->subExpression->dimJagged.num1 == NULL)
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

// to find dimension of rectangular array
void findDimension(parseTreeNode *p)
{
    parseTreeNode *temp = p->child;
    char *num1;
    char *num2;
    while (temp)
    {
        if (isTerminal(temp->nodeData->nodeName))
        {
            temp->nodeData->subExpression->terminal.type = temp->nodeData->nodeName;
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
}

// find DIMENSIONS_REMAINING
void findDimRemaining(parseTreeNode *p)
{
    if (!p)
        return;
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
// to find ids in idlist
void pushID(parseTreeNode *p, newStack *s)
{
    if (!p || !p->child)
    {
        return;
    }
    parseTreeNode *temp = p->child;
    if (temp && isTerminal(temp->nodeData->nodeName))
    {
        newPush(s, temp);
        pushID(temp->next, s); // assuming first child is identifier
    }
    else
    {
        pushID(temp->child, s);
        pushID(temp->next, s);
    }
}

void newPush(newStack *st, parseTreeNode *node)
{
    newStackNode *temp = (newStackNode *)calloc(1, sizeof(newStackNode));
    if (!temp)
    {
        printf("\nHeap Overflow");
        exit(1);
    }
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
    }
}
bool newIsEmpty(newStack *st)
{
    return st->top == NULL;
}

void printDecErrors(int lineNo, int depth, char *message, int *optionalVar)
{
    printf("\n*************************************************************************************\n");
    if (optionalVar)
        printf("%-4d | at depth %-2d | %-15s | %s %d", lineNo, depth, "Declaration", message, *optionalVar);
    else
        printf("%-4d | at depth %-2d | %-15s | %-90s", lineNo, depth, "Declaration", message);
    printf("\n*************************************************************************************\n");
}

void printAssnErrors(parseTreeNode *p1, parseTreeNode *p2, char *op, char *message)
{
    printf("\n*************************************************************************************\n");
    printf("%-4d | at depth %-2d | %-s | %-3s | %-3s | %-3s | %-3s | %-3s | %s", p1->nodeData->lineNo, p1->depth, "Assignment", op, p1->nodeData->nodeName, p1->nodeData->subExpression->data.table.field4.typePrimitive, p2->nodeData->nodeName, p2->nodeData->subExpression->data.table.field4.typePrimitive, message);
    printf("\n*************************************************************************************\n");
}