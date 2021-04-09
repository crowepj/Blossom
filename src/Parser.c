
#include "Parser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//UTILITY

//Prototype in case of a failure and it needs to be called in AST_Parse_xxxx
void AST_Free(struct AST* This);

struct AstNode* MakeValueNode(AstValue value)
{
	struct AstNode* value_node = malloc(sizeof(struct AstNode));

	if (value_node == NULL)
		return NULL;

	value_node->Type = AstValueNode;
	value_node->Children = NULL;

	//If it's a string or identifier copy the value
	if (value.Type == AST_STRING || value.Type == AST_IDENTIFIER)
		value_node->Value.Value.s = strdup(value.Value.s);
	else
		value_node->Value = value;

	return value_node;
};

//Append a value to a node's children
unsigned char AppendChildNode(struct AstNode* target, struct AstNode* value)
{
	//Grow by one
	struct AstNode** Temp = realloc(target->Children, (target->ChildrenLength + 1) * sizeof(struct AstNode));

	if (!Temp)
		return 1;

	//realloc was successful so set the pointer
	target->Children = Temp;
	//Now set the node in the array
	target->Children[target->ChildrenLength] = value;
	//Update size
	target->ChildrenLength++;

	return 0;
}

//Misc
struct AstNode* AST_Parse_Use(struct AST* This, Token* Tokens, int* Index, int TokensSize)
{

}

int expect_token(TokenEnum expected, Token* tokens, int* Index, const char* msg)
{
	if (tokens[*Index + 1].Token == expected)
	{
		*Index += 1;
		return 1;
	}

	else
	{
		if (msg != NULL)
			printf("%s\n", msg);
		return 0;
	}
};

int expect_type(enum AstVariableType expected, Token* tokens, int* Index, const char* msg)
{
	if (tokens[*Index + 1].Type == expected)
	{
		*Index += 1;
		return 1;
	}

	else
	{
		if (msg != NULL)
			printf("%s\n", msg);
		return 0;
	}
};


//Functions and Identifiers

//For parsing function arguments
int AST_Parse_Argument(Token* Tokens, int* Index, int TokensSize, struct AstNode* Call)
{
	while (*Index < TokensSize && Tokens[*Index].Token != CLOSE_BRACKET)
	{
		//Value token is a constant value (String, Float, Char)
		if (expect_token(VALUE, Tokens, Index, NULL))
		{
			struct AstNode* node = MakeValueNode(Tokens[*Index].Value);

			if (node == NULL)
				return -1;

			AppendChildNode(Call, node);
		}

		*Index += 1;
	}

	return 1;
}

struct AstNode* AST_Parse_Function_Call(struct AST* This, Token* Tokens, int* Index, int TokensSize)
{
	struct AstNode* NameNode = MakeValueNode(Tokens[*Index].Value);

	if (!NameNode)
		return NULL;

	NameNode->Children = NULL;
	NameNode->ChildrenLength = 0;
	NameNode->Value.Type = AST_IDENTIFIER;

	struct AstNode* FunctionCallNode = malloc(sizeof(struct AstNode));

	if (!FunctionCallNode)
		return NULL;

	FunctionCallNode->Type = FunctionCall;
	FunctionCallNode->ChildrenLength = 0;
	FunctionCallNode->Children = malloc(0);

	//Append the node that contains the function name
	AppendChildNode(FunctionCallNode, NameNode);
	AST_Parse_Argument(Tokens, Index, TokensSize, FunctionCallNode);

	return FunctionCallNode;
}

struct AstNode* AST_Parse_Identifier(struct AST* This, Token* Tokens, int* Index, int TokensSize)
{
	if (Tokens[*Index + 1].Token == OPEN_BRACKET)
	{
		return AST_Parse_Function_Call(This, Tokens, Index, TokensSize);
	}

	return NULL;
}

unsigned char AST_Parse_FunctionDefinition(struct AST* This, Token* Tokens, int* Index, int TokensSize)
{
	if (Tokens[*Index + 1].Value.Type != AST_STRING)
		return 1;

	This->Name = strdup(Tokens[*Index + 1].Value.Value.s);

	*Index += 2;

	return 0;
}









//Variables stuff


//Parse a constant value like a float, int, string or char
struct AstNode* AST_Parse_Variable_Constant(struct AST* This, Token* Tokens, int* Index, int TokensSize)
{

}

//Parse assigning a variable to another identifier (such as a function call, or just another variable)
struct AstNode* AST_Parse_Variable_Identifier(struct AST* This, Token* Tokens, int* Index, int TokensSize)
{

}

//Parse bracket order e.g if we receive something like: var V := (1 + 1) * 2; Sort out the order and determine which functions to call
struct AstNode* AST_Parse_Variable_Bracket_Order(struct AST* This, Token* Tokens, int* Index, int TokensSize)
{

}

struct AstNode* AST_Parse_Variable(struct AST* This, Token* Tokens, int* Index, int TokensSize)
{

}










void AST_Initialize(struct AST* This)
{
	This->Nodes = malloc(0);
	This->NodeLength = 0;
	This->Name = NULL;
}

void AST_Generate(struct AST* This, Token* Tokens, int TokenSize)
{
	for (int i = 0; i < TokenSize; i++)
	{
		Token Current = Tokens[i];

		switch (Current.Token)
		{
			case IDENTIFIER:
			{
				struct AstNode* node = AST_Parse_Identifier(This, Tokens, &i, TokenSize);

				if (!node)
				{
					return;
				}

				struct AstNode** temp = realloc(This->Nodes, (This->NodeLength + 1) * sizeof(struct AstNode*));

				if (!temp)
				{
					printf("Failed to allocate enough memory\n");
					return;
				}

				This->Nodes = temp;
				This->Nodes[This->NodeLength] = node;
				This->NodeLength++;

				break;
			}
			case FUNCTION:
			{
				if (AST_Parse_FunctionDefinition(This, Tokens, &i, TokenSize))
				{
					printf("Expected identifier after keyword \"func\"\n");
					return;
				}

				break;
			}
				break;
			case USE:
				//AST_Parse_Use(This, Tokens, &i, TokenSize);
				break;
			default:
				break;
		}
	}
}

//Recursive function to clean up nodes
void Free_Node_Recursive(struct AstNode* Node)
{
	if (Node->Children != NULL)
	{
		for (int i = 0; i < Node->ChildrenLength; i++)
		{
			struct AstNode* Current = Node->Children[i];
			Free_Node_Recursive(Current);
		}

		free(Node->Children);
	}

	if (Node->Value.Type == AST_STRING || Node->Value.Type == AST_IDENTIFIER)
	{
		printf("FREE IDENT\n");
		free(Node->Value.Value.s);
	}

	free(Node);
}

void AST_Free(struct AST* This)
{
	for (int i = 0; i < This->NodeLength; i++)
	{
		struct AstNode* CurrentNode = This->Nodes[i];
		Free_Node_Recursive(CurrentNode);
	}

	free(This->Nodes);

	if (This->Name)
		free(This->Name);
};
