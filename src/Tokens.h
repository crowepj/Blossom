#pragma once
typedef enum
{
	VAR,
	VALUE,
	IDENTIFIER,
	FUNCTION,
	OPEN_BRACKET,
	CLOSE_BRACKET,
	OPEN_CURLY_BRACKET,
	CLOSE_CURLY_BRACKET,
	TYPE,
	SEMICOLON,
	EQUAL,
	UNINITIALIZED = -1,
	NULL_TOKEN = -2,
} TokenEnum;

typedef enum
{
	T_INT,
	T_STRING,
	T_CHAR,
	T_FLOAT,
	T_CUSTOM,
} TypeEnum;

typedef struct
{
	TokenEnum Token;
	TypeEnum Type;

	void* Value;
	int Size;
} Token;
