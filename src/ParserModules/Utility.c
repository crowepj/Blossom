#include "Utility.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct AstNode* MakeValueNode(AstValue value)
{
	struct AstNode* value_node = malloc(sizeof(struct AstNode));

	if (value_node == NULL)
		return NULL;

	value_node->Type = AstValueNode;
	value_node->Children = NULL;

	//If it's a string or identifier copy the value
	if (value.Type == AST_STRING || value.Type == AST_IDENTIFIER)
	{
		value_node->Value.Value.s = strdup(value.Value.s);
		value_node->Value.Type = value.Type;
	}
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

AstValue CopyValue(AstValue value)
{
	AstValue ret;
	if (value.Type == AST_STRING || value.Type == AST_IDENTIFIER)
	{
		ret.Type = value.Type;
		ret.Value.s = strdup(value.Value.s);
		return ret;
	}

	return value;
}
