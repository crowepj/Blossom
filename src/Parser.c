
#include "Parser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//Prototype in case of a failure and it needs to be called in AST_Parse_xxxx
void AST_Free(struct AST* This);

//For parsing function arguments, it puts the arguments into the "Call" parameter
void AST_Parse_Argument(struct AST* This, Token* Tokens, int* Index, int TokensSize, struct AstNode* Call)
{
	//Prevent segfaults by checking we're not going over the limit of the array
	while (*Index < TokensSize && Tokens[*Index].Token != SEMICOLON)
	{
		Token Current = Tokens[*Index];
		Token* Next = NULL;

		//Indexing by 0 is the same as (*Index)
		if (Index[0] + 1 < TokensSize)
		{
			Next = &Tokens[Index[0] + 1];
		}

		if (Current.Token == VALUE || Current.Token == IDENTIFIER)
		{
			if (Next != NULL && (*Next).Token != CLOSE_BRACKET && (*Next).Token != COMMA)
			{
				printf("Expected Comma or Closing bracket after function call argument\n");
				return;
			}

			AstValue Value = Current.Value;

			//Reallocate to accomodate value
			void* Temp = realloc(Call->Parameters, sizeof(AstValue) * (Call->ParameterLength + 1));

			//TODO Add AST_Free here
			if (Temp == NULL)
			{
				printf("Something went wrong, please try again\n");
				return;
			}

			//realloc was successful so set it in the call node
			Call->Parameters = Temp;

			//Now set the parameter
			Call->Parameters[Call->ParameterLength] = Value;
			Call->ParameterLength++;

			*Index += 1;
		}

		*Index += 1;
	}
}

void AST_Parse_Use(struct AST* This, Token* Tokens, int* Index, int TokensSize)
{
	char* ModuleNameString = Tokens[*Index].Value.Value.s;

	struct AstNode* Node = malloc(sizeof(struct AstNode));
	Node->Left = NULL;
	Node->Right = NULL;
	Node->Type = Using;

	AstValue ModuleName;
	ModuleName.Value.s = strdup(ModuleNameString);
	ModuleName.Type = AST_IDENTIFIER;

	//Allocate enough space for parameters, and set it
	Node->Parameters = malloc(sizeof(AstValue));
	//Indexing by 0 is the same as *(Node->Parameters), it's just neater
	Node->Parameters[0] = ModuleName;
	Node->ParameterLength = 1;

	Node->Body = NULL;
	Node->BodyLength = 0;

	//Reallocate enough space for nodes and append
	This->Nodes = realloc(This->Nodes, (This->NodeLength * sizeof(struct AstNode*)) + sizeof(struct AstNode*));
	This->Nodes[This->NodeLength] = Node;

	This->NodeLength = This->NodeLength + 1;
}

void AST_Parse_Identifier(struct AST* This, Token* Tokens, int* Index, int TokensSize)
{
	//Access string member of union
	char* FunctionNameString = Tokens[*Index].Value.Value.s;

	Token NextToken = (Tokens[*(Index) + 1]);

	//Expect opening bracket
	if (NextToken.Token == OPEN_BRACKET)
	{
		*Index = (*Index) + 1;

		NextToken = (Tokens[(*Index) + 1]);

		//If it's just an open bracket and a close bracket it's a function with no arguments meaning no extra Parsing has to be done
		if (NextToken.Token == CLOSE_BRACKET)
		{
			//See Parser.h
			struct AstNode* Node = malloc(sizeof(struct AstNode));
			Node->Left = NULL;
			Node->Right = NULL;
			Node->Type = FunctionCall;

			//First parameter of a function call is the function name
			AstValue FunctionName;
			FunctionName.Value.s = strdup(FunctionNameString);
			FunctionName.Type = AST_STRING;

			//Allocate enough space for parameters, and set it
			Node->Parameters = malloc(sizeof(AstValue));
			//Indexing by 0 is the same as *(Node->Parameters), it's just neater
			Node->Parameters[0] = FunctionName;
			Node->ParameterLength = 1;

			Node->Body = NULL;
			Node->BodyLength = 0;

			//Reallocate enough space for nodes and append
			This->Nodes = realloc(This->Nodes, (This->NodeLength * sizeof(struct AstNode*)) + sizeof(struct AstNode*));
			This->Nodes[This->NodeLength] = Node;

			This->NodeLength = This->NodeLength + 1;
		}

		else
		{
			//AstNode defined in Parser.h
			//FIXME Check if malloc returns NULL
			struct AstNode* Node = malloc(sizeof(struct AstNode));
			Node->Left = NULL;
			Node->Right = NULL;
			Node->Type = FunctionCall;

			Node->Body = NULL;
			Node->BodyLength = 0;

			//First parameter of a function call is the function name
			AstValue FunctionName;
			FunctionName.Value.s = strdup(FunctionNameString);
			FunctionName.Type = AST_STRING;

			//Allocate enough space for parameters, and set it
			//	FIXME Check that the result of malloc is not NULL
			Node->Parameters = malloc(sizeof(AstValue));
			//Indexing by 0 is the same as *(Node->Parameters), it's just neater
			Node->Parameters[0] = FunctionName;
			Node->ParameterLength = 1;

			// FIXME Check that the result of realloc is not NULL
			This->Nodes = realloc(This->Nodes, (This->NodeLength * sizeof(struct AstNode*)) + sizeof(struct AstNode*));
			This->Nodes[This->NodeLength] = Node;

			This->NodeLength += 1;

			AST_Parse_Argument(This, Tokens, Index, TokensSize, Node);
		}
	}
}

void AST_Parse_FunctionDefinition(struct AST* This, Token* Tokens, int* Index, int TokensSize)
{
	printf("Function Definition: %s\n", Tokens[(*Index) + 1].Value.Value.s);
	//TODO Check type of token before getting value
	char* FunctionNameString = Tokens[(*Index) + 1].Value.Value.s;
	//TODO Make this actually parse stuff
	*Index += 2;

	This->Name = FunctionNameString;
}

void AST_Initialize(struct AST* This)
{
	This->Nodes = malloc(0);
	This->NodeLength = 0;
}

void AST_Generate(struct AST* This, Token* Tokens, int TokenSize)
{
	for (int i = 0; i < TokenSize; i++)
	{
		Token Current = Tokens[i];

		switch (Current.Token)
		{
			case IDENTIFIER:
				AST_Parse_Identifier(This, Tokens, &i, TokenSize);
				break;
			case FUNCTION:
				AST_Parse_FunctionDefinition(This, Tokens, &i, TokenSize);
				break;
			case USE:
				printf("USE PARSER\n");
				AST_Parse_Use(This, Tokens, &i, TokenSize);
				break;
			default:
				break;
		}
	}
}

//Recursive function to clean up nodes
void Free_Node_Recursive(struct AstNode* Node)
{
	if (Node->Left != NULL)
	{
		Free_Node_Recursive(Node->Left);
		free(Node->Left);
	}

	if (Node->Right != NULL)
	{
		Free_Node_Recursive(Node->Right);
		free(Node->Right);
	}

	if (Node->Body != NULL)
	{
		for (int i = 0; i < Node->BodyLength; i++)
		{
			struct AstNode* Current = Node->Body[i];
			Free_Node_Recursive(Current);
		}

		free(Node->Body);
	}
	if (Node->Parameters != NULL)
	{
		for (int i = 0; i < Node->ParameterLength; i++)
		{
			//The only type that needs to be freed is string
			if (Node->Parameters[i].Type == AST_STRING)
				free(Node->Parameters[i].Value.s);
				asm volatile("nop");
		}

		free(Node->Parameters);
	}

	free(Node);
}

void AST_Free(struct AST* This)
{
	for (int i = 0; i < 1; i++)
	{
		struct AstNode* CurrentNode = This->Nodes[i];
		Free_Node_Recursive(CurrentNode);
	}

	free(This->Nodes);
};
