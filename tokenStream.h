#include <stdio.h>
#include <stdbool.h> 

// #ifndef TOKEN_STREAM_H
// #define TOKEN_STREAM_H

#define MAX_LENGTH 100
#define RULES_LENGTH 100

typedef struct tokenNode {
    char *lexeme;
    char *token;
    int lineNo;
    struct tokenNode *next;
} tokenNode;

typedef struct tokenStream {
	int count;
	struct tokenNode *first;
    struct tokenNode *last;
} tokenStream;

void addToken(tokenStream *head, char* lexemeName, char *tokenName, int line_no);
void tokeniseSourcecode(char *sourcecode, tokenStream *s);
void printTokenStream(tokenStream *s);
char* getTokenName(char* token);
bool isKeyword(char *lexeme);
bool isArithOperator(char *lexeme);
bool isLogicalOperator(char *lexeme);
bool isInteger(char *lexeme);
bool isIdentifier(char *lexeme);

// #endif