#pragma once
#include "DynamicArray.h"

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
	//Primitives
	AST_INTEGER,
	AST_STRING,
	AST_CHAR,

	//Other
	CUSTOM_OBJECT,
};

struct WattObject
{
	enum AstNodeObjectType Type;
	void* Value;
};

struct AstNode
{
	struct AstNode* Left;
	struct AstNode* Right;

	struct DynamicArray* Parameters;
};

//One AST = One Function
struct AST
{
	//DynamicArray is Type <AstNode>
	char* Name;
	struct DynamicArray Nodes;
};

void AST_Initialize(struct AST* This);
void AST_Generate(struct AST* This, struct DynamicArray* Tokens);
