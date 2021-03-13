
#include "Parser.h"
#include "DynamicArray.h"
#include <stdlib.h>

void AST_Parse_Identifier(struct AST* This, struct DynamicArray* Tokens, int* Index) 
{
	char* FunctionNameString = (*(Token*)DynamicArray_At(Tokens, Tokens->Length - 1)).Value;

	Token CurrentToken = *(Token*)DynamicArray_At(Tokens, *Index + 1);

	if (CurrentToken.Token == OPEN_BRACKET) 
	{
		*Index++;

		if (CurrentToken.Token == CLOSE_BRACKET)
		{
			struct WattObject FunctionName;
			FunctionName.Type = STRING;
			FunctionName.Value = _strdup(FunctionNameString);


			struct AstNode* node = malloc(sizeof(struct AstNode));
			node->Left = NULL;
			node->Right = NULL;
			DynamicArray_PushBack(node->Parameters, &FunctionName);
		}
	}
}

void AST_Initialize(struct AST* This, struct DynamicArray Tokens)
{
	This->Root->Parent = NULL;
}