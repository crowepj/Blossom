#pragma once
#include "Tokens.h"
#include "LexerParserDependencies.h"

enum AstNodeType
{
	FunctionCall,
	FunctionDef,
	VariableDeclaration,
	VariableDefinition,
	ConditionalStatement,
	AstValueNode,
	Using,
};

struct AstNode
{
	enum AstNodeType Type;

	struct AstNode** Children;
	int ChildrenLength;

	AstValue Value;
};

//One AST = One Function
struct AST
{
	char* Name;
	struct AstNode** Nodes;
	int NodeLength;
};

void AST_Initialize(struct AST* This);
void AST_Generate(struct AST* This, Token* Tokens, int TokenSize);
void AST_Free(struct AST* This);
