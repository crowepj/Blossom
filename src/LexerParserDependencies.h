#pragma once
#include "Tokens.h"

enum AstVariableType
{
	//Primitives
	AST_INTEGER,
	AST_FLOAT,
	AST_STRING,
	AST_CHAR,

	//Other
	AST_IDENTIFIER,
	CUSTOM_STRUCT,
  AST_NONE
};

typedef struct
{
	enum AstVariableType Type;

	union
	{
		int i;
		float f;
		char* s;
		char c;
	} Value;

} AstValue;

typedef struct
{
	TokenEnum Token;
	enum AstVariableType Type;

	AstValue Value;
} Token;
