#pragma once
#include <Parser.h>
#include <LexerParserDependencies.h>
#include <Tokens.h>

int AST_Parse_Argument(Token* Tokens, int* Index, int TokensSize, struct AstNode* Call);
struct AstNode* AST_Parse_Function_Call(struct AST* This, Token* Tokens, int* Index, int TokensSize);
unsigned char AST_Parse_FunctionDefinition(struct AST* This, Token* Tokens, int* Index, int TokensSize);
