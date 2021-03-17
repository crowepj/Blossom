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
	char** Array = malloc(sizeof(char**));
	Array[0] = malloc(sizeof(char*));

	int ArraySize = 1;
	int ArrayIndex = 0;

	char* Buffer = malloc(sizeof(char));
	Buffer[0] = '\0';

	int InString = 0;

	for (int i = 0; i < strlen(Source); i++)
	{
		char CurrentCharacter = Source[i];

		if (CurrentCharacter == '\"' && Source[i - 1] != '\\')
		{
			InString = !InString;
		}

		if (isdelimeter(CurrentCharacter) && !InString)
		{
			if (CurrentCharacter != ' ')
			{
				concat(CurrentCharacter, &Buffer);
			}

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

	//ArraySize is 1 too big after the loop so subtract 1
	*Size = ArraySize - 1;

	free(Buffer);
	return Array;
}


Token* Lex(char** Tokens, int Size, int* OutSize)
{
	int Index = 0;
	int RetSize = 1;
	Token* RetVal = malloc(sizeof(Token));

	if (RetVal == NULL)
	{
		*OutSize = 0;
		return NULL;
	}

	for (int i = 0; i < Size; i++)
	{
		if (strcmp(Tokens[i], "var") == 0)
		{
			char* Temp = realloc(RetVal, Size);

			if (Temp == NULL)
			{
				printf("Something went wrong, please try again\n");
				free(RetVal);
				return NULL;
			}

			RetVal[Index].Token = VAR;
			RetVal[Index].Value = NULL;
			RetVal[Index].Size = 0;

			Index++;
			RetSize++;
		}
	}

	return RetVal;
}
