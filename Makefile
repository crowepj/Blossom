SRC = $(wildcard src/*.c) $(wildcard src/ParserModules/*.c) $(wildcard src/IntermediateRepModules/*.c)
CFLAGS = -lm -ggdb3 -I src

All:
	gcc $(CFLAGS) $(SRC) -o Interpreter
	./Interpreter
