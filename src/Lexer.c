#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <memory.h>
#include <stdio.h>
#include "Lexer.h"

//For checking if we should split token
int isdelimeter(char c)
{
	if (c == ' ' || c == '(' || c == ')' || c == '=' || c == '\"' || c == '\'' || c == '\0' || c == '\n' || c == ';' || c == ':')
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
	char* temp = (char*)realloc(*destination, strlen(*destination) + 1);

	if (temp == NULL)
	{
		return 0;
	}

	*destination = temp;

	strcat(temp, CurrentCharacter);
	return 1;
}

char** Tokenize(const char* Source, int* Size)
{
	//Array of strings
	//For explanation of malloc(0) see Lex function
	char** Array = malloc(0);

	int ArraySize = 0;
	int ArrayIndex = 0;

	char* Buffer = malloc(sizeof(char));
	Buffer[0] = '\0';

	int InString = 0;

	for (int i = 0; i < strlen(Source); i++)
	{
		char CurrentCharacter = Source[i];

		//Have we hit a quote, is it escaped?, if not, we're either completing a string or starting a string
		if (CurrentCharacter == '\"' && Source[i - 1] != '\\')
		{
			InString = !InString;
		}

		if (isdelimeter(CurrentCharacter) && !InString)
		{
			//Not interested in spaces in the tokens
			if (CurrentCharacter != ' ')
			{
				concat(CurrentCharacter, &Buffer);
			}

			//Not interested if the buffer is empty
			else if(strcmp(Buffer, "") == 0)
			{
				continue;
			}

			//Reallocate array with enough capacity to store pointer to string
			ArraySize++;
			char** ArrTemp = realloc(Array, ArraySize * sizeof(char**));

			if (ArrTemp != NULL)
			{
				//Realloc was successful so set array to it
				Array = ArrTemp;

				//strdup calls malloc in its internal implementation
				Array[ArrayIndex] = strdup(Buffer);

				//printf("Array: %s\nIndex: %i\n", Array[ArrayIndex], ArrayIndex);
			}

			//Otherwise, set it to NULL
			else
			{
				Array[ArrayIndex] = NULL;
			}

			ArrayIndex++;
			//This is the same as memsetting it to 0, this is more efficient because functions like strcmp only go up to a null character, meaning setting the first character is sufficient
			Buffer[0] = '\0';

			continue;
		}

		concat(CurrentCharacter, &Buffer);
	}

	*Size = ArraySize;

	free(Buffer);
	return Array;
}

//Internal helper function for neater code
int MakeToken(Token** TokenArray, TokenEnum TokenE, enum AstVariableType Type, AstValue* Value, int TypeSize, int Index, int TokenArraySize)
{
	Token* TempPtr = realloc(*TokenArray, (TokenArraySize + 1) * sizeof(Token));

	if (TempPtr == NULL)
	{
		printf("Something went wrong, please try again\n");
		free(*TokenArray);
		return 0;
	}

	*TokenArray = TempPtr;
	(*TokenArray)[Index].Token = TokenE;
	(*TokenArray)[Index].Type = Type;

	if (Value != NULL)
		(*TokenArray)[Index].Value = *Value;

	return 1;
}

Token* Lex(char** Tokens, int Size, int* OutSize)
{
	int Index = 0;
	int RetSize = 0;
	//Use malloc to allocate 0 bytes because the pointer is valid if you pass it to realloc
	//These are the tokens to be returned
	Token* RetVal = malloc(0);

	if (RetVal == NULL)
	{
		printf("Failed to allocate buffer, please try again");
		*OutSize = 0;
		return NULL;
	}

	for (int i = 0; i < Size; i++)
	{
		//Lots of if statements
		if (strcmp(Tokens[i], "var") == 0)
		{
			if (MakeToken(&RetVal, VAR, AST_NONE, NULL, 0, Index, RetSize) == 0)
			{
				return NULL;
			}

			Index++;
			RetSize++;
		}

		else if (strcmp(Tokens[i], ":") == 0 && strcmp(Tokens[i + 1], "=") == 0)
		{
			if (MakeToken(&RetVal, EQUAL, AST_NONE, NULL, 0, Index, RetSize) == 0)
			{
				return NULL;
			}

			i += 2;
			Index++;
			RetSize++;
		}

		else if (strcmp(Tokens[i], "func") == 0)
		{
			if (MakeToken(&RetVal, FUNCTION, AST_NONE, NULL, 0, Index, RetSize) == 0)
			{
				return NULL;
			}

			Index++;
			RetSize++;
		}

		else if (strcmp(Tokens[i], "int") == 0)
		{
			if (MakeToken(&RetVal, TYPE, AST_INTEGER, NULL, 0, Index, RetSize) == 0)
			{
				return NULL;
			}

			Index++;
			RetSize++;
		}

		//C string
		else if (strcmp(Tokens[i], "cstr") == 0)
		{
			if (MakeToken(&RetVal, TYPE, AST_STRING, NULL, 0, Index, RetSize) == 0)
			{
				return NULL;
			}

			Index++;
			RetSize++;
		}

		else if (strcmp(Tokens[i], ";") == 0)
		{
			if (MakeToken(&RetVal, SEMICOLON, AST_NONE, NULL, 0, Index, RetSize) == 0)
			{
				return NULL;
			}

			Index++;
			RetSize++;
		}

		else if (strcmp(Tokens[i], ",") == 0)
		{
			if (MakeToken(&RetVal, COMMA, AST_NONE, NULL, 0, Index, RetSize) == 0)
			{
				return NULL;
			}

			Index++;
			RetSize++;
		}

		else if (strcmp(Tokens[i], "(") == 0)
		{
			if (MakeToken(&RetVal, OPEN_BRACKET, AST_NONE, NULL, 0, Index, RetSize) == 0)
			{
				return NULL;
			}

			Index++;
			RetSize++;
		}

		else if (strcmp(Tokens[i], ")") == 0)
		{
			if (MakeToken(&RetVal, CLOSE_BRACKET, AST_NONE, NULL, 0, Index, RetSize) == 0)
			{
				return NULL;
			}

			Index++;
			RetSize++;
		}

		//This means it's an identifier
		else
		{
			char LastCharacter = Tokens[i][strlen(Tokens[i]) - 1];

			//TODO
			//If the last character is a bracket it means it's A. A function Call or B. A function definition, this doesn't matter in the lexer but we have to separate those
			if (LastCharacter == '(')
			{
				//If the last character is an open bracket, we don't want that in the identifier, so set it to a null character
				Tokens[i][strlen(Tokens[i]) - 1] = '\0';

				AstValue val;
				val.Value.s = strdup(Tokens[i]);
				val.Type = AST_STRING;

				if (MakeToken(&RetVal, IDENTIFIER, AST_IDENTIFIER, &val, strlen(Tokens[i]) + 1, Index, RetSize) == 0)
				{
					return NULL;
				}

				Index++;
				RetSize++;

				if (MakeToken(&RetVal, OPEN_BRACKET, AST_NONE, NULL, 0, Index, RetSize) == 0)
				{
					return NULL;
				}

				Index++;
				RetSize++;
			}

			else
			{
				AstValue val;
				val.Value.s = strdup(Tokens[i]);
				val.Type = AST_STRING;

				if (MakeToken(&RetVal, IDENTIFIER, AST_NONE, &val, strlen(Tokens[i]) + 1, Index, RetSize) == 0)
				{
					return NULL;
				}

				Index++;
				RetSize++;
			}
		}

		free(Tokens[i]);
	}


	*OutSize = RetSize;
	free(Tokens);

	return RetVal;
}

void FreeTokens(Token* Tokens, int TokenSize)
{
	for (int i = 0; i < TokenSize; i++)
	{
		if (Tokens[i].Value.Type == AST_STRING)
		{
			free(Tokens[i].Value.Value.s);
		}
	}

	free(Tokens);
}

Token* FullLex(const char* Source, int* TokenSize)
{
	int StringTokenSize = 0;
	char** StringTokens = Tokenize(Source, &StringTokenSize);

	//Argument 1, String Tokens, Argument 2, size of string tokens, Argument 3, Output Variable, Token Array Length
	Token* Tokens = Lex(StringTokens, StringTokenSize, TokenSize);

	return Tokens;
}
