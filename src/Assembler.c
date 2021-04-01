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

//prepend assumes parameters passed are malloc'd
char* prepend(char** original, char* prepend)
{
  //FIXME This is a hack and should be changed when the problem is found
  char* temp = realloc(*original, strlen(prepend) + 40 + strlen(*original));

  if (temp == NULL)
  {
    free(prepend);
    free(*original);
    return NULL;
  }

  *original = temp;

  //Move original up
  memmove(temp + strlen(prepend), *original, strlen(*original));
  memcpy(temp, prepend, strlen(prepend));

  return temp;
};

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
        sprintf(Buffer, "%i",Value.V.i);
        break;
      case IR_FLOAT:
        sprintf(Buffer, "%f",Value.V.f);
        break;
      case IR_STRING:
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
      strcat(Buffer, "\tmov ");
      strcat(Buffer, ABI_Registers[i - 1]);
      strcat(Buffer, ", ");
      strcat(Buffer, ValueAsStr);
      strcat(Buffer, "\n");
    }

    //Otherwise just push to stack
    else
    {
      strcat(Buffer, "\tpush ");
      strcat(Buffer, ValueAsStr);
      strcat(Buffer, "\n");
    }

    free(ValueAsStr);
  }

  strcat(Buffer, "\tcall ");
  strcat(Buffer, opcode.Parameters[0]->V.s);
  strcat(Buffer, "\n");

  return 1;
}

int Asm_GenerateFunctionDef(struct IntermediateRepresentationOp* Opcodes, int Index, int Size, char* Buffer)
{
  //A function/label in nasm is as follows:
  //<Name>:
  //Function Name
  strcat(Buffer, Opcodes[Index].Parameters[0]->V.s);
  strcat(Buffer, ":\n");
  return 1;
}

int Asm_GenerateUse(struct IntermediateRepresentationOp* Opcodes, int Index, int Size, char** Buffer)
{
  char* LibraryName = Opcodes[Index].Parameters[0]->V.s;

  FILE* LibraryFile = fopen(LibraryName, "r");

  if (LibraryFile == NULL)
    return -1;

  char* LibContent = malloc(4096);

  if (LibContent == NULL)
    return -1;

  fread(LibContent, 4096, 1, LibraryFile);

  //TODO Take char** as buffer so that its address can be changed if needed
  prepend(Buffer, LibContent);
  free(LibContent);

  return 1;
}

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
      case USE:
        Asm_GenerateUse(Opcodes, i, Size, &Buffer);
        break;
    }
  }

  //Set up string constants
  //Format is:
  //SCx: db "str",0
  for (int i = 0; i < String_Constants_Length; i++)
  {
    //TODO make this bigger and malloc'd
    char Expression[128];
    //
    char ConstantID[64];
    snprintf(ConstantID, 64, "SC%i", i);

    sprintf(Expression, "%s: db %s,0\n", ConstantID, String_Constants[i]);

    prepend(&Buffer, Expression);
  }
  
  strcat(Buffer, FunctionEnd);

  return Buffer;
}
