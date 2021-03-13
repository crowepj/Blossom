#include "Lexer.h"
#include <stdio.h>

void test(struct DynamicArray Array) 
{
	printf("%i", *(int*)DynamicArray_At(&Array, 0));
}

int main() 
{
	struct DynamicArray lexed = Lex("var test");

	Token* tok = (Token*)DynamicArray_At(&lexed, 1);

	printf("%i", tok->Token);

	DynamicArray_Free(&lexed);

	return 0;
}