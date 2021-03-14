#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <memory.h>
#include <stdio.h>
#include "Lexer.h"

//For checking if we should split token
int isdelimeter(char c)
{
	if (c == ' ' || c == '(' || c == ')' || c == '=' || c == '\"' || c == '\'' || c == '\0' || c == '\n' || c == ';')
	{
		return 1;
	}

	else
	{
		return 0;
	}
};

int isidentifier(const char* buffer)
{
	for (int i = 0; i < strlen(buffer); i++)
	{
		if (isalpha(buffer[i]) == 0) return 0;
	}

	return 1;
};

int concat(char c, char** destination)
{
	char CurrentCharacter[] = { c,'\0' };

	//Reallocate for enough space
	char* temp = (char*)realloc(*destination, strlen(*destination) + 2);

	if (temp == NULL)
	{
		return 0;
	}

	*destination = temp;

	strcat(*destination, CurrentCharacter);
}

//Return shallow copy of Token List, gotta free it yourself though ;)
struct DynamicArray Lex(const char* Source)
{
	struct DynamicArray Tokens;
	DynamicArray_Initialize(&Tokens, sizeof(Token));

	enum State CurrentState = NONE;
	char* Buffer = malloc(sizeof(char));
	*Buffer = '\0';

	for (int i = 0; i < strlen(Source) + 1; i++)
	{
		char current = Source[i];

		//Identify Current State
		switch (current)
		{
		case '\"':
			if (CurrentState == IN_QUOTE)
			{
				CurrentState = NONE;

				Token Tok;
				Tok.Value = strdup(Buffer);
				Tok.Size = strlen(Buffer) + 1;
				Tok.Type = STRING;
				Tok.Token = VALUE;

				DynamicArray_PushBack(&Tokens, &Tok);

				Buffer[0] = '\0';

				continue;
			}

			else
			{
				CurrentState = IN_QUOTE;
				continue;
			}
			break;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			if (CurrentState != IN_QUOTE && CurrentState != IN_SINGLE_QUOTE)
			{
				CurrentState = IN_NUMBER;
			}
			break;
		}

		//If delimeter reached, make token if not in char and not in string
		if (CurrentState != IN_QUOTE && CurrentState != IN_SINGLE_QUOTE && isdelimeter(current))
		{
			if (CurrentState == IN_NUMBER)
			{
				int* ValuePointer = malloc(sizeof(int));
				*ValuePointer = atoi(Buffer);

				Token Tok;
				Tok.Value = ValuePointer;
				Tok.Size = sizeof(int);
				Tok.Type = INT;
				Tok.Token = VALUE;

				DynamicArray_PushBack(&Tokens, &Tok);

				Buffer[0] = '\0';
				continue;
			}

			else if (strcmp(Buffer, "var") == 0)
			{
				Token Tok;
				Tok.Type = NONE;
				Tok.Token = VAR;
				Tok.Value = NULL;

				DynamicArray_PushBack(&Tokens, &Tok);
				Buffer[0] = '\0';
				continue;
			}

			else if (strcmp(Buffer, "func") == 0)
			{
				Token Tok;
				Tok.Type = NONE;
				Tok.Token = FUNCTION;
				Tok.Value = NULL;

				DynamicArray_PushBack(&Tokens, &Tok);
				Buffer[0] = '\0';
				continue;
			}

			else if (strcmp(Buffer, "int") == 0)
			{
				Token Tok;
				Tok.Type = INT;
				Tok.Token = TYPE;
				Tok.Value = NULL;

				DynamicArray_PushBack(&Tokens, &Tok);
				Buffer[0] = '\0';
				continue;
			}

			else if (isidentifier(Buffer))
			{
				Token Tok;
				Tok.Type = NONE;
				Tok.Token = IDENTIFIER;
				Tok.Value = strdup(Buffer);
				Tok.Size = strlen(Buffer) + 1;

				DynamicArray_PushBack(&Tokens, &Tok);
				Buffer[0] = '\0';
				continue;
			}
		}

		if (current == ';')
		{
			Token Tok;
			Tok.Token = SEMICOLON;
			Tok.Value = NULL;

			DynamicArray_PushBack(&Tokens, &Tok);
			continue;
		}

		concat(current, &Buffer);
		printf(Buffer);
		printf("\n");
	}

	free(Buffer);

	return Tokens;
}
