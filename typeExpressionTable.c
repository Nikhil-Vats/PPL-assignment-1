#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "typeExpressionTable.h"

int populateTag(enum dataInfo f2, char *f3)
{
    // printf("check f2 = %d and f3 = %s\n",f2,f3);
    if (f2 == primitive)
    {
        return 0;
    }
    if (f2 == rectangular && strcmp(f3, "static") == 0)
        return 1;
    if (f2 == rectangular && strcmp(f3, "dynamic") == 0)
        return 2;
    return 3;
}
void addType(tableData data, typeExpressionTable *T)
{
    if (T->count == T->capacity)
    {
        T->dataTypeExpr = (tableData *)realloc(T->dataTypeExpr, sizeof(tableData) * (T->capacity * 2));
    }
    // printf("added type");
    T->dataTypeExpr[T->count++] = data;
    // printf("\njust added data for %s which is of type %d\n",T->dataTypeExpr[T->count-1].field1,T->dataTypeExpr[T->count-1].field2);
}

void printTypeExpressionTable(typeExpressionTable *T)
{
    printf("%-8s | %8s | %20s | %s\n", "Field1", "Field2", "Field3", "Field4");
    for (int i = 0; i < T->count; i++)
    {
        if (T->dataTypeExpr[i].tag == 0)
        {
            char *ty = "primitive";
            printf("\n*************************************************************************************\n");
            printf("%-8s | %8s | %20s | <type=%s>", T->dataTypeExpr[i].field1, ty, T->dataTypeExpr[i].field3, T->dataTypeExpr[i].field4.typePrimitive);
            printf("\n*************************************************************************************\n");
        }
        else if (T->dataTypeExpr[i].tag == 1 || T->dataTypeExpr[i].tag == 2)
        {
            char *ty = "rectangularArray";
            char r[10] = ",range_R";
            // char p[10];
            char c;
            char *f;
            // for (int j = 0; j < T->dataTypeExpr[i].field4.rect.dim; j++)
            // {
                // c = '0' + j;
                // p[j] = c;
            // }

            printf("\n*************************************************************************************\n");
            printf("%-2s | %-2s | %-20s | <type=%s ,dimensions=%d ,", T->dataTypeExpr[i].field1, ty, T->dataTypeExpr[i].field3, T->dataTypeExpr[i].field4.rect.type, T->dataTypeExpr[i].field4.rect.dim);
            for (int j = 0; j < T->dataTypeExpr[i].field4.rect.dim; j++)
            {
                // c = '0' + j;
                // p[j] = c;
                printf("range_R%d=(%s,%s),", j+1 ,T->dataTypeExpr[i].field4.rect.R[j][0], T->dataTypeExpr[i].field4.rect.R[j][1]);

            }
            printf(",basicElementType=%s>", T->dataTypeExpr[i].field4.rect.basicElementType);
            printf("\n*************************************************************************************\n");
        }
        else if(T->dataTypeExpr[i].tag == 3)
        {
            char *ty = "jaggedArray";
            printf("\n*************************************************************************************\n");
            if(T->dataTypeExpr[i].field4.jag.dim == 2) {
                printf("%-2s | %-2s | %-20s | <type=%s, dimensions=%d, range_R1=(%s,%s), range_R2= (", T->dataTypeExpr[i].field1, ty, T->dataTypeExpr[i].field3, T->dataTypeExpr[i].field4.jag.type, T->dataTypeExpr[i].field4.jag.dim, T->dataTypeExpr[i].field4.jag.R1[0], T->dataTypeExpr[i].field4.jag.R1[1]);
                // T->dataTypeExpr[i].field4.jag._2dor3d.r2[0],T->dataTypeExpr[i].field4.jag._2dor3d.r2[1],T->dataTypeExpr[i].field4.jag._2dor3d.r2[2],T->dataTypeExpr[i].field4.jag._2dor3d.r2[3],T->dataTypeExpr[i].field4.jag._2dor3d.r2[4],T->dataTypeExpr[i].field4.jag._2dor3d.r2[5],T->dataTypeExpr[i].field4.jag._2dor3d.r2[6]);
                int rows = (int) atoi(T->dataTypeExpr[i].field4.jag.R1[1]) - (int) atoi(T->dataTypeExpr[i].field4.jag.R1[0]) + 1;
                // printf("%d",rows);
                for(int j = 0; j < rows - 1; j++) {
                    printf("%d,", T->dataTypeExpr[i].field4.jag._2dor3d.r2[j]);
                }
                printf("%d), basicElementType=%s>", T->dataTypeExpr[i].field4.jag._2dor3d.r2[rows - 1], T->dataTypeExpr[i].field4.jag.basicElementType);
            } else {
                printf("%-2s | %-2s | %-20s | <type=%s, dimensions=%d, range_R1=(%s,%s), range_R2= (", T->dataTypeExpr[i].field1, ty, T->dataTypeExpr[i].field3, T->dataTypeExpr[i].field4.jag.type, T->dataTypeExpr[i].field4.jag.dim, T->dataTypeExpr[i].field4.jag.R1[0], T->dataTypeExpr[i].field4.jag.R1[1]);
                int rows = (int) atoi(T->dataTypeExpr[i].field4.jag.R1[1]) - (int) atoi(T->dataTypeExpr[i].field4.jag.R1[0]) + 1;
                // printf("%d\n", T->dataTypeExpr[i].field4.jag._2dor3d.r2[1]);
                for(int m = 0; m < rows; m++) {
                    printf("%d [ ",T->dataTypeExpr[i].field4.jag._2dor3d.r2[m]);
                    for(int n = 0; n < T->dataTypeExpr[i].field4.jag._2dor3d.r2_[m].r; n++) {
                        if(n == (T->dataTypeExpr[i].field4.jag._2dor3d.r2_[m].r - 1)) {
                            printf("%d ",T->dataTypeExpr[i].field4.jag._2dor3d.r2_[m].arr[n]);
                        } else {
                            printf("%d, ",T->dataTypeExpr[i].field4.jag._2dor3d.r2_[m].arr[n]);
                        }
                    }
                    if(m == (rows - 1)) {
                        printf("]");
                    } else {
                        printf("], ");
                    }
                }
                printf("), basicElementType=%s>", T->dataTypeExpr[i].field4.jag.basicElementType);

            }
            printf("\n*************************************************************************************\n");
        }
    }
}