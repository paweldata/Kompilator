all: parser lexer
	g++ -o compiler parser.tab.cpp lex.yy.c -lfl

lexer:
	flex lexer.l

parser:
	bison -d parser.ypp

clean:
	rm -f parser.tab.* lex.*
