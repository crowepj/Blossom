#pragma once
#include "DynamicArray.h"

///Forward Declare Struct to prevent circular inclusion
struct ParameterList;

enum AstNodeType 
{
	FunctionCall,
	FunctionDeclaration,
	VariableDeclaration,
	VariableDefinition,
	ConditionalStatement,
};

enum AstNodeObjectType 
{
	AST_INTEGER,
	AST_STRING,
	AST_CHAR,
	CUSTOM_OBJECT,
	///For Parameter List first value, so that it can now that the first value is uninitialized
	AST_UNINITIALIZED,
};

struct WattObject
{
	enum AstNodeObjectType Type;
	void* Value;
};

struct AstNode
{
	struct AstNode* Parent;
	struct AstNode* Left;
	struct AstNode* Right;

	struct ParameterList* Parameters;
};

struct AST 
{
	struct AstNode* Root;
};

void AST_Initialize(struct AST* This, struct DynamicArray Tokens);