#include "Lexer.h"
#include "Parser.h"
#include "IntermediateRepresentation.h"
#include "Assembler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t GetFileSize(FILE* _File)
{
	fseek(_File, 0, SEEK_END);
	size_t len = ftell(_File);
	rewind(_File);

	return len;
}

int ReadSourceFile(char* FPath, char** SourceOut, size_t* Length)
{
	FILE* _File = fopen(FPath, "r");
	char* _Source = NULL;

	if (_File == NULL)
		return -1;

	size_t _File_Length = GetFileSize(_File);

	_Source = malloc(_File_Length);

	if (_Source == NULL)
	{
		fclose(_File);
		return -1;
	}

	fread(_Source, _File_Length, 1, _File);

	if (_Source == NULL)
	{
		fclose(_File);
		return -1;
	}

	*SourceOut = _Source;
	*Length = _File_Length;

	fclose(_File);

	return 1;
}

int WriteSourceFile(char* FPath, char* Input, size_t InputLength)
{
	FILE* _File = fopen(FPath, "w+");

	if (_File == NULL)
		return -1;

	fprintf(_File, Input);

	fclose(_File);
	return 1;
}

int main()
{
	char* SourceFileCode = NULL;
	size_t len = 0;

	ReadSourceFile("testing.sap", &SourceFileCode, &len);

	int TokenSize;

	Token* Tokens = FullLex(SourceFileCode, len, &TokenSize);
	struct AST SyntaxTree;

	AST_Initialize(&SyntaxTree);
	AST_Generate(&SyntaxTree, Tokens, TokenSize);

	int Opsize = 0;
	struct IntermediateRepresentationOp* Opcodes;
	Opcodes = GenerateIR(&SyntaxTree, &Opsize);

	char* assembled = Assemble(Opcodes, Opsize);

	printf(assembled);

	FreeIR(Opcodes, Opsize);
	AST_Free(&SyntaxTree);
	FreeTokens(Tokens, TokenSize);
	free(assembled);
	free(SourceFileCode);
	CleanupAssembler();
	return 0;
}
