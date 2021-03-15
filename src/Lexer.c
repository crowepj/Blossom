#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <memory.h>
#include <stdio.h>
#include "Lexer.h"

//A table which describes the tranisitions between Tokens
static enum FiniteStateTransition FiniteStateTransitionTable[8][8];

void Initialize_Lexer()
{
	FiniteStateTransitionTable[NONE][REJECTED] = NONE;
	FiniteStateTransitionTable[NONE][NONE] = NONE;
	FiniteStateTransitionTable[NONE][INTEGER] = INTEGER;
	FiniteStateTransitionTable[NONE][OPERATOR] = OPERATOR;
	FiniteStateTransitionTable[NONE][CHARACTER] = CHARACTER;
	FiniteStateTransitionTable[NONE][SPACE] = SPACE;
	FiniteStateTransitionTable[NONE][DELIMITER] = DELIMITER;

	FiniteStateTransitionTable[INTEGER][REJECTED] = REJECTED;
	FiniteStateTransitionTable[INTEGER][NONE] = NONE;
	FiniteStateTransitionTable[INTEGER][INTEGER] = INTEGER;
	FiniteStateTransitionTable[INTEGER][OPERATOR] = OPERATOR;
	FiniteStateTransitionTable[INTEGER][CHARACTER] = REJECTED;
	FiniteStateTransitionTable[INTEGER][SPACE] = NONE;
	FiniteStateTransitionTable[INTEGER][DELIMITER] = DELIMITER;

	FiniteStateTransitionTable[OPERATOR][REJECTED] = REJECTED;
	FiniteStateTransitionTable[OPERATOR][NONE] = NONE;
	FiniteStateTransitionTable[OPERATOR][INTEGER] = INTEGER;
	FiniteStateTransitionTable[OPERATOR][OPERATOR] = OPERATOR;
	FiniteStateTransitionTable[OPERATOR][CHARACTER] = CHARACTER;
	FiniteStateTransitionTable[OPERATOR][SPACE] = NONE;
	FiniteStateTransitionTable[OPERATOR][DELIMITER] = DELIMITER;

	FiniteStateTransitionTable[CHARACTER][REJECTED] = REJECTED;
	FiniteStateTransitionTable[CHARACTER][NONE] = CHARACTER;
	FiniteStateTransitionTable[CHARACTER][INTEGER] = CHARACTER;
	FiniteStateTransitionTable[CHARACTER][OPERATOR] = CHARACTER;
	FiniteStateTransitionTable[CHARACTER][CHARACTER] = CHARACTER;
	FiniteStateTransitionTable[CHARACTER][SPACE] = CHARACTER;
	FiniteStateTransitionTable[CHARACTER][DELIMITER] = DELIMITER;

	//This has to be here for it to work
	FiniteStateTransitionTable[INTEGER][SPACE] = NONE;

	printf("\n%i\n\n", FiniteStateTransitionTable[INTEGER][SPACE]);
}

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
	char* temp = (char*)realloc(*destination, strlen(*destination) + 2);

	if (temp == NULL)
	{
		return 0;
	}

	*destination = temp;

	strcat(*destination, CurrentCharacter);
}

enum FiniteStateTransition GetFiniteState(char Character)
{
	if (isdelimeter(Character))
	{
		return DELIMITER;
	}

	else if (isalpha(Character) != 0)
	{
		return CHARACTER;
	}

	else if (isdigit(Character) != 0)
	{
		return INTEGER;
	}

	else if (ispunct(Character) != 0)
	{
		printf("\n%c\n\n", Character);
		return OPERATOR;
	}

	return NONE;
};

const char* GetName(enum FiniteStateTransition Token)
{
	switch (Token)
	{
		case REJECTED:
			return "REJECTED";
			break;
		case NONE:
			return "NONE";
			break;
		case INTEGER:
			return "INTEGER";
			break;
		case OPERATOR:
			return "OPERATOR";
			break;
		case CHARACTER:
			return "CHARACTER";
			break;
		case SPACE:
			return "SPACE";
			break;
		case DELIMITER:
			return "DELIMITER";
			break;
		case QUOTE:
			return "QUOTE";
			break;
		default:
			return "UNDEFINED";
			break;
	}
};

//Return shallow copy of Token List, gotta free it yourself though ;)
struct DynamicArray Lex(const char* Source)
{
	struct DynamicArray e;

	enum FiniteStateTransition Result = NONE;
	enum FiniteStateTransition CurrentState = NONE;
	enum FiniteStateTransition PreviousState = NONE;

	char* Buffer = malloc(sizeof(char));

	for (int i = 0; i < strlen(Source); i++)
	{
		char CurrentCharacter = Source[i];
		CurrentState = GetFiniteState(CurrentCharacter);

		Result = FiniteStateTransitionTable[PreviousState][CurrentState];

		//printf("%c : %i\n", CurrentCharacter, Result);

		if (Result == DELIMITER)
		{
			printf("%s\n", Buffer);
			CurrentState = NONE;
			PreviousState = NONE;
			Result = NONE;
			Buffer[0] = '\0';
			continue;
		}

		CurrentState = Result;
		PreviousState = CurrentState;
		Result = NONE;

		concat(CurrentCharacter, &Buffer);
	}

	free(Buffer);
	return e;
}
