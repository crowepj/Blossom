#include "Assembler.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <AssemblerModules/Utility.h>
#include <AssemblerModules/Variable.h>
#include <AssemblerModules/Function.h>

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

int CleanupAssembler()
{
  for (int i = 0; i < String_Constants_Length; i++)
  {
    free(String_Constants[i]);
  }

  free(String_Constants);
};

int Asm_GenerateUse(struct IntermediateRepresentationOp* Opcodes, int Index, int Size, char** Buffer)
{
  char* LibraryName = Opcodes[Index].Children[0]->Value.V.s;

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

char* Assemble(struct IntermediateRepresentationOp* Opcodes, int Size)
{
  //For setting up and cleaning stack
  char* FunctionStart = "\tpush rbp\n\r\tmov rbp, rsp\n\r";
  char* FunctionEnd = "\tpop rbp\n\r\tret\n\r";

  char* Buffer = malloc(4096);

  if (Buffer == NULL)
    return NULL;

  sprintf(Buffer, "\0");
  strcat(Buffer, "global _start\n");

  for (int i = 0; i < Size; i++)
  {
    printf("STRING CONSTANT LENGTH: %i\n\n", String_Constants_Length);
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
      case V_ASSIGN:
        printf("VARIABLE ASSIGN\n");
        Asm_GenerateVariableConstant(Opcodes, i, Size, Buffer);
        break;
    }
  }

  strcat(Buffer, FunctionEnd);

  printf("STRING CONSTANT LENGTH: %i\n\n", String_Constants_Length);

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
