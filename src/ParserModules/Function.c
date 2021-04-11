#include <stdlib.h>
#include <string.h>
#include "Utility.h"
#include "Function.h"

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

unsigned char AST_Parse_FunctionDefinition(struct AST* This, Token* Tokens, int* Index, int TokensSize)
{
	if (Tokens[*Index + 1].Value.Type != AST_STRING)
		return 1;

	This->Name = strdup(Tokens[*Index + 1].Value.Value.s);

	*Index += 2;

	return 0;
}
