#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #include "parseTree.h"
// #include "typeExpressionTable.h"

#define Max_DIMENSION 10
#define MAX_ENTRIES 20
#define MAX_ROWS 10

typedef struct TERMINAL
{
    char *type;
} TERMINAL;

typedef struct DIMENSIONS
{
    int count; //number of dims. dimension count -> DIMENSIONS + (DIMENSIONS_REMAINING-1)
    char *ranges[Max_DIMENSION][2];
} DIMENSIONS;

typedef struct DIMENSION
{
    char *range[2];
} DIMENSION;

typedef struct DIMENSIONS_REMAINING
{
    int count; //dimension count -> DIMENSION + (DIMENSIONS_REMAINING-1)
    char *ranges[Max_DIMENSION][2];
} DIMENSIONS_REMAINING;

typedef struct DIMJAGGED
{
    char *num1;
    char *num2;
    int dim; //dimensions from no of sq brackets
} DIMJAGGED;

typedef struct RANGE
{
    char *num1; // check later for static or dynamic
    char *num2;
} RANGE;

typedef struct NUMLIST
{
    int count; // number of entries
    char *listofnum[MAX_ENTRIES];
} NUMLIST;

typedef struct NUMS
{
    int count; // number of entries
    char *num[MAX_ENTRIES];
} NUMS;

typedef struct LISTOFNUMLIST
{
    int count; // number of entries of nums
    //char* listofnums[MAX_ROWS][MAX_ENTRIES];
    struct NUMS listofnums[MAX_ROWS];
} LISTOFNUMLIST;

// R1 [ 4 ] : size 3 : values { 21 641 23 36 125 ; 54 221 43 ; 287 501 453 334 23 }
// MAKEROWS R1 SQUAREOP NUM1 SQUARECL COLON size NUM COLON values CURLYOP NUMLIST CURLYCL MAKEROWS2
typedef struct MAKEROWS
{
    int rows;          // total no of rows
    char **listofrows; // list of R1
    char **size;       //NUM
    int is3D;          // tag: 0 -> 2D, 1->3D
    union
    {
        struct NUMLIST *Numlist;
        struct LISTOFNUMLIST *Listofnumlist;
    } listselect;
} MAKEROWS;

typedef struct newStackNode
{
    struct parseTreeNode *node;
    struct newStackNode *next;
} newStackNode;

typedef struct newStack
{
    int count;
    struct newStackNode *top;
} newStack;