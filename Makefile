all: compile clean

compile: parser lexer CodeGenerator.o Operations.o Conditions.o FlowControler.o Command.o Array.o Memory.o
	g++ -g3 -o compiler parser.tab.cpp lex.yy.c -lfl CodeGenerator.o Operations.o Conditions.o FlowControler.o Command.o Array.o Memory.o

lexer:
	flex lexer.l

parser:
	bison -d parser.ypp

CodeGenerator.o: src/CodeGenerator.cpp src/CodeGenerator.h
	g++ -g3 -c src/CodeGenerator.cpp -o CodeGenerator.o

Operations.o: src/Operations.cpp src/CodeGenerator.h
	g++ -g3 -c src/Operations.cpp -o Operations.o

Conditions.o: src/Conditions.cpp src/CodeGenerator.h
	g++ -g3 -c src/Conditions.cpp -o Conditions.o

FlowControler.o: src/FlowControler.cpp src/CodeGenerator.h
	g++ -g3 -c src/FlowControler.cpp -o FlowControler.o

Command.o: src/Command.cpp src/Command.h
	g++ -g3 -c src/Command.cpp -o Command.o

Memory.o: src/Memory.cpp src/Memory.h
	g++ -g3 -c src/Memory.cpp -o Memory.o

Array.o: src/variable/Array.cpp src/variable/Array.h
	g++ -g3 -c src/variable/Array.cpp -o Array.o

clean:
	rm -f parser.tab.* lex.* *.o
