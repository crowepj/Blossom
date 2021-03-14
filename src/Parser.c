
#include "Parser.h"
#include "DynamicArray.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void AST_Parse_Identifier(struct AST* This, struct DynamicArray* Tokens, int* Index)
{
	char* FunctionNameString = (*(Token*)DynamicArray_At(Tokens, Tokens->Length - 1)).Value;

	Token NextToken = *(Token*)DynamicArray_At(Tokens, *Index + 1);

	//Expect opening bracket
	if (NextToken.Token == OPEN_BRACKET)
	{
		*Index++;

		NextToken = *(Token*)DynamicArray_At(Tokens, *Index + 1);

		//Function that takes no parameters
		if (NextToken.Token == CLOSE_BRACKET)
		{
			struct WattObject FunctionName;
			FunctionName.Type = STRING;
			FunctionName.Value = strdup(FunctionNameString);

			struct AstNode* node = malloc(sizeof(struct AstNode));
			node->Left = NULL;
			node->Right = NULL;
			//The name of the function is the first parameter in the AstNode
			DynamicArray_PushBack(node->Parameters, &FunctionName);
		}
	}
}

void AST_Parse_Var(struct AST* This, struct DynamicArray* Tokens, int* Index)
{
	Token NextToken = *(Token*)DynamicArray_At(Tokens, *Index + 1);

	//[VAR] <Identifier> = <Value>
	if (NextToken.Token == IDENTIFIER)
	{
		*Index++;
		NextToken = *(Token*)DynamicArray_At(Tokens, *Index + 1);

		if (NextToken.Token == EQUAL)
		{
			*Index++;
			NextToken = *(Token*)DynamicArray_At(Tokens, *Index + 1);

			if (NextToken.Token == VALUE)
			{

			}

			else
			{
				printf("Expected Value after Equal\n");
			}
		}
	}

	else
	{
		printf("Expected Identifier after keyword Var\n");
	}
}

void AST_Initialize(struct AST* This)
{
	DynamicArray_Initialize(&This->Nodes, sizeof(struct AstNode));
}

void AST_Generate(struct AST* This, struct DynamicArray* Tokens)
{
	for (int i = 0; i < Tokens->Length; i++)
	{
		Token Current = *(Token*)DynamicArray_At(Tokens, i);
		switch (Current.Token)
		{
			case IDENTIFIER:
				AST_Parse_Identifier(This, Tokens, &i);
				break;
			case VAR:

				break;
		}
	}
}
