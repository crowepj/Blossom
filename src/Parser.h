#pragma once
#include "Tokens.h"
#include "LexerParserDependencies.h"

enum AstNodeType
{
	FunctionCall,
	FunctionDeclaration,
	VariableDeclaration,
	VariableDefinition,
	ConditionalStatement,
	AstValueNode,
};

struct AstNode
{
	struct AstNode* Left;
	struct AstNode* Right;

	AstValue* Parameters;
	int ParameterLength;

	//If not initialized to NULL then this will be used as a container of the statements in a block of code
	struct AstNode** Body;
	int BodyLength;
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
