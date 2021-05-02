#include "Variable.h"
#include "Utility.h"

size_t GetTypeSize(struct IntermediateRepresentationValue val)
{
  switch (val.T)
  {
    case IR_FLOAT:
      return 8;
      break;
    case IR_INT:
      return 4;
      break;
    case IR_STRING:
        return 8;
        break;
  }
};

//TODO Implement variables as some sort of hash map
size_t getVariableOffset(char* name)
{
  if (strcmp(name,"retval") == 0)
    return 8;

  for (int i = 0; i < VariableLength; i++)
  {
    if (strcmp(Variables[i].name, name) == 0)
      return Variables[i].stack_off;
  }

  return 0;
}

int AddVariable(char* name, size_t off)
{
  struct _VariableType var;
  var.name = name;

  if (off == 0)
    return 0;

  var.stack_off = MostRecentStackOffset += off;
  var.size = off;

  struct _VariableType* Temp = realloc(Variables, sizeof(struct _VariableType) * (VariableLength + 1));

  if (!Temp)
    return -1;

  Variables = Temp;
  Variables[VariableLength++] = var;
  VariableLength++;

  return MostRecentStackOffset;
};

int Asm_GenerateVariableConstant(struct IntermediateRepresentationOp* Opcodes, int Index, int Size, char* Buffer)
{
  //TODO Type checking
  if (Opcodes[Index].Children[1]->Value.T == IR_IDENT)
  {
    char* instruction = malloc(1024);

    if (!instruction)
      return -1;

    instruction[0] = '\0';

    size_t off = getVariableOffset(Opcodes[Index].Children[1]->Value.V.s);

    if (off == 0)
      return -1;

    sprintf(instruction, "\tmov qword rdi,[rsp-%i]\n\tmov [rsp-%i], rdi\n",MostRecentStackOffset+4 ,off);
    AddVariable(Opcodes[Index].Children[0]->Value.V.s, 4);

    strcat(Buffer, instruction);
    free(instruction);

    return 1;
  }

  size_t typeSize = GetTypeSize(Opcodes[Index].Children[1]->Value);
  char* asString = GetValueString(Opcodes[Index].Children[1]->Value);

  char* instruction = malloc(1024);

  if (!instruction)
    return -1;

  instruction[0] = '\0';

  //TODO Check type is identifier
  AddVariable(strdup(Opcodes[Index].Children[0]->Value.V.s), typeSize);

  sprintf(instruction, "\tmov qword [rsp-%i],%s\n",MostRecentStackOffset,asString);

  strcat(Buffer, instruction);
  free(instruction);

  return 1;
}

int Asm_GenerateVariableFunctionCall(struct IntermediateRepresentationOp* Opcodes, int Index, int Size, char* Buffer)
{

}

int Asm_GenerateVariable_Assign_Variable(struct IntermediateRepresentationOp* Opcodes, int Index, int Size, char* Buffer)
{

}

int Asm_GenerateVariableIdentifier(struct IntermediateRepresentationOp* Opcodes, int Index, int Size, char* Buffer)
{
  return Asm_GenerateVariable_Assign_Variable(Opcodes, Index, Size, Buffer);
}
