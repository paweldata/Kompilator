all: parser lexer CodeGenerator.o CodeWriter.o
	g++ -o compiler parser.tab.cpp lex.yy.c -lfl CodeGenerator.o CodeWriter.o

lexer:
	flex lexer.l

parser:
	bison -d parser.ypp

CodeGenerator.o: src/CodeGenerator.cpp src/CodeGenerator.h
	g++ -c src/CodeGenerator.cpp -o CodeGenerator.o

CodeWriter.o: src/CodeWriter.cpp src/CodeWriter.h
	g++ -c src/CodeWriter.cpp -o CodeWriter.o

clean:
	rm -f parser.tab.* lex.*
