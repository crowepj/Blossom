#include "Lexer.h"
#include "Parser.h"
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

	int TokenSize;

	Token* Tokens = FullLex("test(4542) ", &TokenSize);
	struct AST SyntaxTree;

	printf("OUT\n");

	AST_Initialize(&SyntaxTree);
	AST_Generate(&SyntaxTree, Tokens, TokenSize);

	AST_Free(&SyntaxTree);
	FreeTokens(Tokens, TokenSize);
	return 0;
}
