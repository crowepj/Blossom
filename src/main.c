#include "Lexer.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
	FILE* File = fopen("art","r");

	if (File != NULL)
	{
		fseek(File, 0, SEEK_END);
		long size = ftell(File);
		fseek(File, 0, SEEK_SET);

		char* str = malloc(size + 1);

		if (str != NULL)
		{
			fread(str, 1, size, File);

			printf(str);
			free(str);
		}

		fclose(File);
	}

	Initialize_Lexer();
	struct DynamicArray E = Lex("int test = 3");

	for (int i = 0; i < E.Length; i++)
	{
		Token* tok = DynamicArray_At(&E, i);
		free((void*)tok->Original);
	}

	DynamicArray_Free(&E);
	return 0;
}
