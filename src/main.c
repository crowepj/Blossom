#include "Lexer.h"
#include "Parser.h"
#include "IntermediateRepresentation.h"
#include "Assembler.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
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
