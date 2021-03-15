#include "Lexer.h"
#include <stdio.h>

int main()
{
	Initialize_Lexer();
	struct DynamicArray E = Lex("int test = 3");
	return 0;
}
