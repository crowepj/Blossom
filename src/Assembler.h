#pragma once
#include <stddef.h>
#include "IntermediateRepresentation.h"

//64 bit ABI Registers are the registers in which parameters should be moved into
//There are 6 of them, any more arguments are pushed onto the stack
/*
    First Argument: RDI
    Second Argument: RSI
    Third Argument: RDX
    Fourth Argument: RCX
    Fifth Argument: R8
    Sixth Argument: R9
*/
static char* ABI_Registers[] =
{
  "RDI",
  "RSI",
  "RDX",
  "RCX",
  "R8",
  "R9",
};
static char** String_Constants = NULL;
static int String_Constants_Length = 0;

char* GetValueString(struct IntermediateRepresentationValue Value);
char* Assemble(struct IntermediateRepresentationOp* Opcodes, int Size);
