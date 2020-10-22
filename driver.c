#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grammar.h"
#include "tokenStream.h"

int main() {
    grammar G;
    tokenStream *s = (tokenStream*)malloc(sizeof(tokenStream));
    readGrammar("grammar.txt",G);
    tokeniseSourcecode("code.txt",s);
    printTokenStream(s);
    return 0;
}