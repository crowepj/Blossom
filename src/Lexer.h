#pragma once
#include "LexerParserDependencies.h"
#include "Tokens.h"

//Size is the length of the outputted array, it is set in the function
char** Tokenize(const char* Source, int* Size);

//Tokens is the value returned from Tokenize, Size is the size returned from Tokenize,
//OutSize is the size of the Token array returned by this function
Token* Lex(char** Tokens, int Size, int* OutSize);

void FreeTokens(Token* Tokens, int TokenSize);

Token* FullLex(const char* Source, int* TokenSize);
