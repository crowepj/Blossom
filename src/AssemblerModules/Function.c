#include "Function.h"
#include "Utility.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//Buffer is assumed to be 1024 bytes
int Asm_GenerateFunctionCall(struct IntermediateRepresentationOp* Opcodes, int Index, int Size, char* Buffer)
{
  struct IntermediateRepresentationOp opcode = Opcodes[Index];

  //Parameter 0 is the function name
  for (int i = 1; i < opcode.ChildrenLength; i++)
  {
    if (Opcodes[Index].Children[i]->Opcode == IR_VALUE)
    {
      printf("VALUE PARAMETER\n");
      char* ValueAsStr = GetValueString((opcode.Children[i])->Value);

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
  }

  char* Temp = malloc(strlen(opcode.Children[0]->Value.V.s) + 6 + 1 + 1);

  if (!Temp)
    return -1;

  sprintf(Temp, "\tcall %s\n", opcode.Children[0]->Value.V.s);
  strcat(Buffer, Temp);
  free(Temp);

  return 1;
}

int Asm_GenerateFunctionDef(struct IntermediateRepresentationOp* Opcodes, int Index, int Size, char* Buffer)
{
  //A function/label in nasm is as follows:
  //<Name>:
  //Function Name
  char* Temp = malloc(strlen(Opcodes[Index].Children[0]->Value.V.s) + 3);
  if (Temp == NULL) return -1;

  sprintf(Temp, "%s:\n", Opcodes[Index].Children[0]->Value.V.s);
  strcat(Buffer, Temp);

  free(Temp);
  return 1;
}
