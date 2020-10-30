run : driver.o grammar.o tokenStream.o parseTree.o traverseParseTree.o typeExpressionTable.o -o exe

driver.o : driver.c
	gcc -c driver.c

grammar.o : grammar.h grammar.c
	gcc -c grammar.c

tokenStream.o : tokenStream.h tokenStream.c
	gcc -c tokenStream.c

parseTree.o : parseTree.h parseTree.c
	gcc -c parseTree.c

traverseParseTree.o : parseTree.h traverseParseTree.c
	gcc -c traverseParseTree.c

typeExpressionTable.o : typeExpressionTable.h typeExpressionTable.c
	gcc -c typeExpressionTable.c