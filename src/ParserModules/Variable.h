#pragma once
#include <Parser.h>
#include <LexerParserDependencies.h>
#include <Tokens.h>

//Parse assigning a variable to another identifier (such as a function call, or just another variable)
struct AstNode* AST_Parse_Variable_Identifier(struct AST* This, Token* Tokens, int* Index, int TokensSize);

//Parse bracket order e.g if we receive something like: var V := (1 + 1) * 2; Sort out the order and determine which functions to call
struct AstNode* AST_Parse_Variable_Bracket_Order(struct AST* This, Token* Tokens, int* Index, int TokensSize);

struct AstNode* AST_Parse_Variable(struct AST* This, Token* Tokens, int* Index, int TokensSize);
