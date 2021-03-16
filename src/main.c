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
			if (fread(str, 1, size, File) != NULL && str != NULL)
			{
				printf(str);
			}

			free(str);
		}

		fclose(File);
	}

	int Size;
	char** E = Tokenize("int test = 3 ", &Size);

	for (int i = 0; i < Size - 1; i++)
	{
		if (E[i] != NULL)
		{
			free(E[i]);
		}
	}

	printf("%i\n", Size);

	free(E);
	return 0;
}
