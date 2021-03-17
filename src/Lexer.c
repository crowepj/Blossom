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
			//Allocate enough space to store string
			char* Temp = malloc(strlen(Buffer) + 1);

			if (Temp != NULL && ArrTemp != NULL)
			{
				//Realloc was successful so set array to it
				Array = ArrTemp;
				//Add new string value
				Array[ArrayIndex] = Temp;
				//Copy value over
				strcpy(Array[ArrayIndex], Buffer);
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

	//Repeat the block because the last token is lost if there isn't a delimiter
	if (Buffer[0] != '\0')
	{
		//Reallocate array with enough capacity to store pointer to string
		ArraySize++;
		char** ArrTemp = realloc(Array, ArraySize * sizeof(char**));
		//Allocate enough space to store string
		char* Temp = malloc(strlen(Buffer) + 1);

		if (Temp != NULL && ArrTemp != NULL)
		{
			//Realloc was successful so set array to it
			Array = ArrTemp;
			//Add new string value
			Array[ArrayIndex] = Temp;
			//Copy value over
			strcpy(Array[ArrayIndex], Buffer);
		}

		//Otherwise, set it to NULL
		else
		{
			Array[ArrayIndex] = NULL;
		}

		ArrayIndex++;
		Buffer[0] = '\0';
	}

	*Size = ArraySize;

	free(Buffer);
	return Array;
}

//Internal helper function for neater code
int MakeToken(Token** TokenArray, TokenEnum TokenE, TypeEnum Type, void* Value, int TypeSize, int Index, int TokenArraySize)
{
	Token* TempPtr = realloc(*TokenArray, TokenArraySize + 1);

	if (TempPtr == NULL)
	{
		printf("Something went wrong, please try again\n");
		free(*TokenArray);
		return 0;
	}

	*TokenArray = TempPtr;
	(*TokenArray)[Index].Token = TokenE;
	(*TokenArray)[Index].Type = Type;
	(*TokenArray)[Index].Value = Value;
	(*TokenArray)[Index].Size = TypeSize;

	return 1;
}

Token* Lex(char** Tokens, int Size, int* OutSize)
{
	int Index = 0;
	int RetSize = 0;
	//Use malloc to allocate 0 bytes because the pointer is valid if you pass it to realloc
	//These are the tokens to be returned
	Token* RetVal = malloc(0);

	if (RetVal == 0)
	{
		printf("Failed to allocate buffer, please try again");
		*OutSize = 0;
		return NULL;
	}

	for (int i = 0; i < Size; i++)
	{
		//Lots of if statements
		//TODO: Put token enum in order and have an array of strings that are also in order, iterate through them and compare, might be slower(O(n)) but it's much neater
		if (strcmp(Tokens[i], "var") == 0)
		{
			if (MakeToken(&RetVal, VAR, T_NONE, NULL, 0, Index, RetSize) == 0)
			{
				return NULL;
			}

			Index++;
			RetSize++;
		}

		else if (strcmp(Tokens[i], "func") == 0)
		{
			if (MakeToken(&RetVal, FUNCTION, T_NONE, NULL, 0, Index, RetSize) == 0)
			{
				return NULL;
			}

			Index++;
			RetSize++;
		}

		else if (strcmp(Tokens[i], "int") == 0)
		{
			if (MakeToken(&RetVal, TYPE, T_INT, NULL, 0, Index, RetSize) == 0)
			{
				return NULL;
			}

			Index++;
			RetSize++;
		}

		else if (strcmp(Tokens[i], ";") == 0)
		{
			if (MakeToken(&RetVal, SEMICOLON, T_NONE, NULL, 0, Index, RetSize) == 0)
			{
				return NULL;
			}

			Index++;
			RetSize++;
		}
	}

	*OutSize = RetSize;

	return RetVal;
}
