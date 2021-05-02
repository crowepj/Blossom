SRC = $(wildcard src/*.c) $(wildcard src/ParserModules/*.c) $(wildcard src/IntermediateRepModules/*.c) $(wildcard src/AssemblerModules/*.c)
CFLAGS = -lm -ggdb3 -I src

All:
	gcc $(CFLAGS) $(SRC) -o Interpreter
	./Interpreter
