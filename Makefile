all: compile clean

compile: parser lexer CodeGenerator.o Command.o Variable.o Memory.o
	g++ -g3 -o compiler parser.tab.cpp lex.yy.c -lfl CodeGenerator.o Command.o Variable.o Memory.o

lexer:
	flex lexer.l

parser:
	bison -d parser.ypp

CodeGenerator.o: src/CodeGenerator.cpp src/CodeGenerator.h
	g++ -g3 -c src/CodeGenerator.cpp -o CodeGenerator.o

Command.o: src/Command.cpp src/Command.h
	g++ -g3 -c src/Command.cpp -o Command.o

Memory.o: src/Memory.cpp src/Memory.h
	g++ -g3 -c src/Memory.cpp -o Memory.o

Variable.o: src/Variable.cpp src/Variable.h
	g++ -g3 -c src/Variable.cpp -o Variable.o

clean:
	rm -f parser.tab.* lex.* *.o
