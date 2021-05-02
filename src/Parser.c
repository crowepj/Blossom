
#include "Parser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ParserModules/Function.h>
#include <ParserModules/Utility.h>
#include <ParserModules/Variable.h>

//UTILITY

//Prototype in case of a failure and it needs to be called in AST_Parse_xxxx
void AST_Free(struct AST* This);

//Misc
struct AstNode* AST_Parse_Use(struct AST* This, Token* Tokens, int* Index, int TokensSize)
{

}

struct AstNode* AST_Parse_Identifier(struct AST* This, Token* Tokens, int* Index, int TokensSize)
{
	if (Tokens[*Index + 1].Token == OPEN_BRACKET)
	{
		return AST_Parse_Function_Call(This, Tokens, Index, TokensSize);
	}

	else if (Tokens[*Index + 1].Token == EQUAL || Tokens[*Index].Token == EQUAL)
	{
		return AST_Parse_Variable(This, Tokens, Index, TokensSize);
	}

	return NULL;
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
			case VAR:
			{
				struct AstNode* node = AST_Parse_Variable(This, Tokens, &i, TokenSize);

				if (!node)
					return;

				struct AstNode** temp = realloc(This->Nodes, (This->NodeLength + 1) * sizeof(struct AstNode*));

				if (!temp)
				{
					printf("Failed to allocate enough memory\n");
					return;
				}

				This->Nodes = temp;
				This->Nodes[This->NodeLength] = node;
				This->NodeLength++;
			}
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
