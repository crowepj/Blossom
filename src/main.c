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

int main(int argc, char** argv)
{
	char* entrypoint = argv[1];

	char* SourceFileCode = NULL;
	size_t len = 0;

	ReadSourceFile("src/testing.sap", &SourceFileCode, &len);

	int TokenSize;

	Token* Tokens = FullLex(SourceFileCode, len, &TokenSize);
	struct AST SyntaxTree;

	AST_Initialize(&SyntaxTree);
	AST_Generate(&SyntaxTree, Tokens, TokenSize);

	int Opsize = 0;
	struct IntermediateRepresentationOp* Opcodes;
	Opcodes = GenerateIR(&SyntaxTree, &Opsize);

	char* assembled = Assemble(Opcodes, Opsize);


	assembled = realloc(assembled, strlen(assembled) + 200);

	strcat(assembled, "extern printf;");

	WriteSourceFile("src/out/main.asm", assembled, strlen(assembled));
	system("nasm src/out/main.asm -o src/out/main.o -f elf64; ld src/out/main.o -lc -dynamic-linker /usr/lib64/ld-linux-x86-64.so.2 -e _start -o a.out;");

	FreeIR(Opcodes, Opsize);
	AST_Free(&SyntaxTree);
	FreeTokens(Tokens, TokenSize);
	free(assembled);
	free(SourceFileCode);
	return 0;
}
