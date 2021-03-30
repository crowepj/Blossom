#include "Lexer.h"
#include "Parser.h"
#include "IntermediateRepresentation.h"
#include "Assembler.h"
#include <stdio.h>
#include <stdlib.h>

//little endian
int main()
{
	//NOTE TO FUTURE SELF, SEGFAULT IS IN PARSER USE
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

	char* SourceFileCode = malloc(2048);
	FILE* SourceFile = fopen("testing.sap", "r");

	fseek(SourceFile, 0, SEEK_END);
	int len = ftell(SourceFile);
	rewind(SourceFile);

	fread(SourceFileCode, len, 1, SourceFile);
	fclose(SourceFile);

	int TokenSize;

	Token* Tokens = FullLex(SourceFileCode, len, &TokenSize);
	struct AST SyntaxTree;

	AST_Initialize(&SyntaxTree);
	AST_Generate(&SyntaxTree, Tokens, TokenSize);

	int OpLength;
	struct IntermediateRepresentationOp* Opcodes = GenerateIR(&SyntaxTree, &OpLength);

	char* final = Assemble(Opcodes, OpLength);
	printf(final);
	free(final);

	FreeIR(Opcodes, OpLength);
	AST_Free(&SyntaxTree);
	FreeTokens(Tokens, TokenSize);
	free(SourceFileCode);
	return 0;
}
