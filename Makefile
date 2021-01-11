all: compile clean

compile: parser lexer CodeGenerator.o Operations.o Conditions.o Command.o Variable.o Array.o ArrayAddress.o Constant.o Memory.o
	g++ -g3 -o compiler parser.tab.cpp lex.yy.c -lfl CodeGenerator.o Operations.o Conditions.o Command.o Variable.o Array.o ArrayAddress.o Constant.o Memory.o

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

Command.o: src/Command.cpp src/Command.h
	g++ -g3 -c src/Command.cpp -o Command.o

Memory.o: src/Memory.cpp src/Memory.h
	g++ -g3 -c src/Memory.cpp -o Memory.o

Variable.o: src/variable/Variable.cpp src/variable/Variable.h
	g++ -g3 -c src/variable/Variable.cpp -o Variable.o

Array.o: src/variable/Array.cpp src/variable/Array.h
	g++ -g3 -c src/variable/Array.cpp -o Array.o

ArrayAddress.o: src/variable/ArrayAddress.cpp src/variable/ArrayAddress.h
	g++ -g3 -c src/variable/ArrayAddress.cpp -o ArrayAddress.o

Constant.o: src/variable/Constant.cpp src/variable/Constant.h
	g++ -g3 -c src/variable/Constant.cpp -o Constant.o

clean:
	rm -f parser.tab.* lex.* *.o
