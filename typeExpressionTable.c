#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "typeExpressionTable.h"

int populateTag(enum dataInfo f2, char *f3)
{
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