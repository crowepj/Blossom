#include "Lexer.h"
#include <stdio.h>
#include <stdlib.h>
//little endian
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
	int TokenSize;
	char** E = Tokenize("printf();", &Size);

	Token* T = Lex(E, Size, &TokenSize);

	for (int i = 0; i < Size; i++)
	{
		printf("%s\n",E[i]);
		if (E[i] != NULL)
		{
			free(E[i]);
		}
	}

	free(E);
	free(T);
	return 0;
}
