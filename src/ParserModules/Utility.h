#pragma once
#include <Parser.h>
#include <LexerParserDependencies.h>
#include <Tokens.h>

struct AstNode* MakeValueNode(AstValue value);
unsigned char AppendChildNode(struct AstNode* target, struct AstNode* value);
AstValue CopyValue(AstValue value);

int expect_token(TokenEnum expected, Token* tokens, int* Index, const char* msg);
int expect_type(enum AstVariableType expected, Token* tokens, int* Index, const char* msg);
