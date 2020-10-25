#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tokenStream.h"

void addToken(tokenStream *s, char* lexemeName, char *tokenName, int line_no) {
    //create a link
    tokenNode *node = (tokenNode *)malloc(sizeof(tokenNode));

    node->lexeme = malloc(strlen(lexemeName) + 1);
    node->token = malloc(strlen(tokenName) + 1);

    strcpy(node->lexeme, lexemeName);
    strcpy(node->token, tokenName);
    node->lineNo = line_no;
    node->next = NULL;

    if (s->first == NULL && s->last == NULL) {
        s->first = s->last = node;
    } else {
        s->last->next = node;
        s->last = node;
    }
    s->count++;
    return;
}

void tokeniseSourcecode(char *sourcecode, tokenStream *s) {
    FILE* file = fopen(sourcecode, "r");
    if(file == NULL) {
        printf("Error opening file!");
        exit(1);
    }
    char line[256];
    int line_no = 1;
    while (fgets(line, sizeof(line), file)) {
        char *token;
        token = strtok(line, " \n");
        while (token != NULL)
        {   
            char* tokenName = getTokenName(token);
            addToken(s, token, tokenName, line_no);
            token = strtok(NULL, " \n");
        }
        line_no++;
    }
}

void printTokenStream(tokenStream *s) {
    tokenNode *temp = s->first;
    while(temp != NULL) {
        printf("%s ",temp->lexeme);
        printf("%s ",temp->token);
        printf("%d ",temp->lineNo);
        printf("\n");
        temp = temp->next;
    }
}

char* getTokenName(char* token) {
    char* tokenName = "default";
    if(isKeyword(token))
    tokenName = token;
    else if(isArithOperator(token)) {
        if(token[0] == '+')
        tokenName = "plus_op";
        else if(token[0] == '-')
        tokenName = "minus_op";
        else if(token[0] == '*')
        tokenName = "mul_op";
        else if(token[0] == '/')
        tokenName = "div_op";
        else if(token[0] == '=')
        tokenName = "assign_op";
    } else if(isLogicalOperator(token)) {
        if(strcmp(token,"&&&") == 0)
        tokenName = "logic_and";
        else if(strcmp(token,"|||") == 0)
        tokenName = "logic_or";
    } else if(isInteger(token)) {
        tokenName = "static_const";
    } else if(isIdentifier(token)) {
        tokenName = "ID";
    } else if(strlen(token) == 1) { // { } [ ] ; :
        if(token[0] == '{')
        tokenName = "CURLYOP";
        else if(token[0] == '}')
        tokenName = "CURLYCL";
        else if(token[0] == '[')
        tokenName = "SQUAREOP";
        else if(token[0] == ']')
        tokenName = "SQUARECL";
        else if(token[0] == ';')
        tokenName = "semicol";
        else if(token[0] == ':')
        tokenName = "colon";
    } else if(strlen(token) == 2) { // ..  ()
        if(strcmp(token,"()") == 0)
        tokenName = "PARENTHESES";
        else if(strcmp(token,"..") == 0)
        tokenName = "range_dots";
    }
    return tokenName;
}

bool isKeyword(char *lexeme) {
    bool result = false;
    if(strcmp(lexeme,"program") == 0 || strcmp(lexeme,"declare") == 0
       || strcmp(lexeme,"list") == 0 || strcmp(lexeme,"of") == 0
       || strcmp(lexeme,"variables") == 0 || strcmp(lexeme,"array") == 0
       || strcmp(lexeme,"size") == 0 || strcmp(lexeme,"values") == 0
       || strcmp(lexeme,"jagged") == 0 || strcmp(lexeme,"integer") == 0
       || strcmp(lexeme,"real") == 0 || strcmp(lexeme,"boolean") == 0) {
           return true;
       }

    return result;
}

bool isArithOperator(char *lexeme) {
    bool result = false;

    if(strlen(lexeme) == 1 && (lexeme[0] == '+' || lexeme[0] == '-' || lexeme[0] == '*' || lexeme[0] == '/' || lexeme[0] == '='))
    result = true;

    return result;
}

bool isLogicalOperator(char *lexeme) {
    bool result = false;

    if(strlen(lexeme) == 3 && (strcmp(lexeme,"&&&") == 0 || strcmp(lexeme,"|||") == 0))
    result = true;

    return result;
}

bool isInteger(char *lexeme) {
    bool result = true;

    for(int i = 0; i < strlen(lexeme); i++) {
        if(lexeme[i] < '0' || lexeme[i] > '9') {
            result = false;
            break;
        }
    }

    return result;
}

bool isIdentifier(char *lexeme) {
    bool result = true;
    if(strlen(lexeme) > 20 || (lexeme[0] >= '0' && lexeme[0] <= '9')) {
        return false;
    }
    for(int i = 0; i < strlen(lexeme); i++) {
        if((lexeme[i] >= '0' && lexeme[i] <= '9') 
            || (lexeme[i] >= 'a' && lexeme[i] <= 'z')
            || (lexeme[i] >= 'A' && lexeme[i] <= 'Z')
            || lexeme[i] == '_') {
            continue;
        } else {
            return false;
        }
    }

    return result;
}