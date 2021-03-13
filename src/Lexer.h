#pragma once
#include "DynamicArray.h"

enum State
{
	TERMINAL,
	NON_TERMINAL,

	IN_QUOTE,
	IN_SINGLE_QUOTE,

	IN_NUMBER,

	NONE,
};

struct DynamicArray Lex(const char* Source);