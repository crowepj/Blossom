#pragma once
#include "DynamicArray.h"

enum FiniteStateTransition
{
	REJECTED,
	NONE,
	INTEGER,
	OPERATOR,
	CHARACTER,
	SPACE,
	DELIMITER,
	QUOTE,
};

void Initialize_Lexer();
struct DynamicArray Lex(const char* Source);
