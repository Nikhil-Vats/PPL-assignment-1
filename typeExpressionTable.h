#include <stdio.h>
#include <stdbool.h>

#define MAX_DIM 10
#define MAX_VARS 100

/////typeExpr for rectangular
typedef struct RECT
{
    char *type; //="rectangularArray";
    int dim;
    char *R[MAX_DIM][2];    // 1 to dim
    char *basicElementType; //="integer";
} RECT;

typedef struct jagged3d
{
    int r;       /// dim 2 size
    int *arr; //dim 3 vals
} jagged3d;

/////typeExpr for jagged
typedef struct jaggedDataExp
{
    char *type; //="jaggedArray";
    int dim;
    char* R1[2];//////1st dimension ranges
    //dim is the tag here. if dim =2    then use r2
    //  if dim =3 use jagged3d
    char *basicElementType; //="integer";
    union
    {
        int *r2; ///dim 2 vals
        jagged3d *r2_;
    } _2dor3d;

} jaggedDataExp;

enum dataInfo
{
    primitive,
    rectangular,
    jagged
};

/////main structure
typedef struct tableData
{
    //char* typeExpression;
    char *field1;         ///name of var
    enum dataInfo field2; // datatype
    char *field3;         //static or dyanamic for rectuangular or non applicable
    /*
    field2 = 0 =>primitive  union mein typeprimitive
    field2 =1 => rectangular  & field3= "static" 
    field2 =1 => rectangular  & field3= "dyanamic" 
    field2=2 => jagged 
    */
    int tag; // 0 1 2
    union
    {
        char *typePrimitive;
        RECT rect;
        jaggedDataExp jag;
    } field4;
} tableData;

typedef struct typeExpressionTable
{
    tableData *dataTypeExpr;
    int count;
    int capacity;
} typeExpressionTable;

int populateTag(enum dataInfo f2, char *f3);
void addType(tableData d,typeExpressionTable *data);