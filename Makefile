all: compile clean

compile: parser lexer CodeGenerator.o CodeWriter.o Command.o Variable.o Memory.o
	g++ -g3 -o compiler parser.tab.cpp lex.yy.c -lfl CodeGenerator.o CodeWriter.o Command.o Variable.o Memory.o

lexer:
	flex lexer.l

parser:
	bison -d parser.ypp

CodeGenerator.o: src/CodeGenerator.cpp src/CodeGenerator.h
	g++ -c src/CodeGenerator.cpp -o CodeGenerator.o

CodeWriter.o: src/CodeWriter.cpp src/CodeWriter.h
	g++ -c src/CodeWriter.cpp -o CodeWriter.o

Command.o: src/Command.cpp src/Command.h
	g++ -c src/Command.cpp -o Command.o

Memory.o: src/Memory.cpp src/Memory.h
	g++ -c src/Memory.cpp -o Memory.o

Variable.o: src/Variable.cpp src/Variable.h
	g++ -c src/Variable.cpp -o Variable.o

clean:
	rm -f parser.tab.* lex.* *.o
