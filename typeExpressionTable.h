#include <stdio.h>
#include <stdbool.h>

#define MAX_DIM 10
#define MAX_VARS 100

// typeExpr for rectangular arrays
typedef struct RECT
{
    char *type; // "rectangularArray";
    int dim;
    char *R[MAX_DIM][2];
    char *basicElementType; // "integer";
} RECT;

typedef struct jagged3d
{
    int r;       // dim 2 size
    int *arr; // dim 3 vals
} jagged3d;

// typeExpr for jagged arrays
typedef struct jaggedDataExp
{
    char *type; // "jaggedArray";
    int dim; //dim is the tag here
    char* R1[2]; // 1st dimension ranges
    char *basicElementType; //="integer";
    union
    {
        int *r2; // if dim = 2, dim 2 vals
        jagged3d *r2_; // if dim = 3, use jagged3d
    } _2dor3d;

} jaggedDataExp;

enum dataInfo
{
    primitive,
    rectangular,
    jagged
};

// main structure
typedef struct tableData
{
    char *field1;         // name of var
    enum dataInfo field2; // datatype
    char *field3;         // static or dynamic for rectuangular or non applicable
    bool isWrong;
    /*
    field2 = 0 => primitive
    field2 = 1 => rectangular & field3= "static" 
    field2 = 1 => rectangular & field3= "dyanamic" 
    field2 = 2 => jagged 
    */
    int tag; // calculated based on field 2 and field 3
    union
    {
        char *typePrimitive; // tag = 0
        RECT rect; // tag = 1 or 2
        jaggedDataExp jag; // tag = 3
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