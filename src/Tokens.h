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
	COLON,
	SEMICOLON,
	EQUAL,
	UNINITIALIZED = -1,
	NULL_TOKEN = -2,
} TokenEnum;

///This matches the definition in Parser.h, except for T_NONE
typedef enum
{
	T_INT,
	T_FLOAT,
	T_STRING,
	T_CHAR,

	//Other
	T_STRUCT,

	T_NONE,
} TypeEnum;

typedef struct
{
	TokenEnum Token;
	TypeEnum Type;

	void* Value;
	int Size;
} Token;
