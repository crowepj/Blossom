#include "Assembler.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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
char* ABI_Registers[] =
{
  "RDI",
  "RSI",
  "RDX",
  "RCX",
  "R8",
  "R9",
};

char** String_Constants = NULL;
int String_Constants_Length = 0;

char* GetValueString(struct IntermediateRepresentationValue Value)
{
  //TODO change this size dynamically
  char* Buffer = malloc(128);

  if (Buffer == NULL)
    return NULL;

    //TODO Add more
    switch (Value.T)
    {
      case IR_INT:
        printf("INTEGER\n");
        printf("VALUE: %i\n", Value.V.i);
        sprintf(Buffer, "%i",Value.V.i);
        break;
      case IR_FLOAT:
        sprintf(Buffer, "%f",Value.V.f);
        break;
      case IR_STRING:
        printf("STRING CONSTANT\n");
        if (String_Constants == NULL)
        {
          String_Constants = malloc(sizeof(char*));

          if (String_Constants == NULL)
          {
            free(Buffer);
            return NULL;
          }
        }

        String_Constants[String_Constants_Length] = malloc(strlen(Value.V.s) + 1);

        if (String_Constants[String_Constants_Length] == NULL)
        {
          free(Buffer);
          return NULL;
        }

        strcpy(String_Constants[String_Constants_Length], Value.V.s);

        sprintf(Buffer,"SC%i", String_Constants_Length);

        String_Constants_Length++;
        break;
      case IR_IDENT:
        printf("IDENTIFIER\n");
        snprintf(Buffer, 128, Value.V.s);
        break;
    }

    return Buffer;
}

//Buffer is assumed to be 1024 bytes
int Asm_GenerateFunctionCall(struct IntermediateRepresentationOp* Opcodes, int Index, int Size, char* Buffer)
{
  struct IntermediateRepresentationOp opcode = Opcodes[Index];

  //Parameter 0 is the function name
  for (int i = 1; i < opcode.ParametersLength; i++)
  {
    char* ValueAsStr = GetValueString(*(opcode.Parameters[i]));

    if (ValueAsStr == NULL)
      return -1;


    //Use 64 bit ABI Registers
    if (i - 1 < 6)
    {
      //Allocate space for instruction string
      char* Temp = malloc(5 + 3 + 2 + strlen(ValueAsStr));

      if (Temp == NULL) return -1;

      sprintf(Temp, "\tmov %s,%s\n", ABI_Registers[i - 1], ValueAsStr);
      strcat(Buffer, Temp);
      free(Temp);
    }

    //Otherwise just push to stack
    else
    {
      char* Temp = malloc(6 + 2 + strlen(ValueAsStr) + 1);

      if (Temp == NULL) return -1;

      sprintf(Temp, "\tpush %s\n", ValueAsStr);
      strcat(Buffer, Temp);
      free(Temp);
    }

    free(ValueAsStr);
  }

  char* Temp = malloc(strlen(opcode.Parameters[0]->V.s) + 6 + 1 + 1);

  if (!Temp)
    return -1;

  sprintf(Temp, "\tcall %s\n", opcode.Parameters[0]->V.s);
  strcat(Buffer, Temp);
  free(Temp);

  return 1;
}

int Asm_GenerateFunctionDef(struct IntermediateRepresentationOp* Opcodes, int Index, int Size, char* Buffer)
{
  //A function/label in nasm is as follows:
  //<Name>:
  //Function Name
  char* Temp = malloc(strlen(Opcodes[Index].Parameters[0]->V.s) + 3);
  if (Temp == NULL) return -1;

  sprintf(Temp, "%s:\n", Opcodes[Index].Parameters[0]->V.s);
  strcat(Buffer, Temp);

  free(Temp);
  return 1;
}

int Asm_GenerateUse(struct IntermediateRepresentationOp* Opcodes, int Index, int Size, char** Buffer)
{
  char* LibraryName = Opcodes[Index].Parameters[0]->V.s;

  FILE* LibraryFile = fopen(LibraryName, "r");

  if (LibraryFile == NULL)
    return -1;

  char* LibContent = malloc(4096);

  if (LibContent == NULL) return -1;

  fread(LibContent, 4096, 1, LibraryFile);

  strcat(*Buffer, LibContent);
  free(LibContent);

  fclose(LibraryFile);

  return 1;
}

int CleanupAssembler()
{
  for (int i = 0; i < String_Constants_Length; i++)
  {
    free(String_Constants[i]);
  }

  free(String_Constants);
};

char* Assemble(struct IntermediateRepresentationOp* Opcodes, int Size)
{
  //For setting up and cleaning stack
  char* FunctionStart = "\tpush rbp\n\r\tmov rbp, rsp\n\r";
  char* FunctionEnd = "\tpop rbp\n\r\tret\n\r";

  char* Buffer = malloc(4096);

  if (Buffer == NULL)
    return NULL;

  sprintf(Buffer, "\0");

  for (int i = 0; i < Size; i++)
  {
    struct IntermediateRepresentationOp Op = Opcodes[i];

    switch (Op.Opcode)
    {
      case CALL:
        Asm_GenerateFunctionCall(Opcodes, i, Size, Buffer);
        break;
      case FUNCDEF:
        Asm_GenerateFunctionDef(Opcodes, i, Size, Buffer);
        strcat(Buffer, FunctionStart);
        break;
    }
  }

  strcat(Buffer, FunctionEnd);

  //Set up string constants
  //Format is:
  //SCx:db "str",0
  for (int i = 0; i < String_Constants_Length; i++)
  {
    //SCx:db y,0\n
    //Allocate enough length for the string, the symbols, and extra for good luck
    char* Expression = malloc(strlen(String_Constants[i]) + 1 + 10 + 2 + 2);
    //

    char ConstantID[64];
    snprintf(ConstantID, 64, "SC%i", i);
    sprintf(Expression, "%s:db %s,0\n", ConstantID, String_Constants[i]);

    strcat(Buffer, Expression);

    free(Expression);
  }

  CleanupAssembler();

  return Buffer;
}
