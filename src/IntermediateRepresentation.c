#include "IntermediateRepresentation.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//Convert from an AST Value to an IR Value
struct IntermediateRepresentationValue GetValue(AstValue Value)
{
  if (Value.Type != AST_NONE)
  {
    struct IntermediateRepresentationValue l_Value;
    l_Value.T = (enum IntermediateRepresentationType)Value.Type;

    switch (Value.Type)
    {
      case AST_INTEGER:
        l_Value.V.i = Value.Value.i;
        break;
      case AST_IDENTIFIER:
      case AST_STRING:
        printf("IDENTIFIER/STRING: %s\n", Value.Value.s);
        //Duplicate so that this stage doesn't have a dependency on the AST being not freed
        l_Value.V.s = strdup(Value.Value.s);
        break;
      case AST_FLOAT:
        l_Value.V.f = Value.Value.f;
        break;
    }

    return l_Value;
  }

  //FIXME Return some value to indicate failure
}

struct IntermediateRepresentationOp GenerateIR_FunctionDef(struct AST* Tree)
{
  printf("DEF IR\n");
  struct IntermediateRepresentationOp Def;
  Def.Opcode = FUNCDEF;
  Def.ParametersLength = 1;


  //FIXME check malloc doesn't return NULL
  struct IntermediateRepresentationValue* value = malloc(sizeof(struct IntermediateRepresentationValue));
  value->T = IR_IDENT;
  value->V.s = strdup(Tree->Name);
  //FIXME Check malloc doesn't return NULL
  Def.Parameters = malloc(sizeof(struct IntermediateRepresentationValue*));
  Def.Parameters[0] = value;

  return Def;
}

struct IntermediateRepresentationOp GenerateIR_Use(struct AstNode* Node)
{
  struct IntermediateRepresentationOp Use;
  Use.Opcode = IR_USE;
  Use.ParametersLength = 1;

  struct IntermediateRepresentationValue* value = malloc(sizeof(struct IntermediateRepresentationValue));
  value->T = IR_STRING;
  value->V.s = strdup(Node->Parameters[0].Value.s);

  Use.Parameters = malloc(sizeof(struct IntermediateRepresentationValue*));
  Use.Parameters[0] = value;

  return Use;
};

struct IntermediateRepresentationOp GenerateIR_FunctionCall(struct AST* Tree, struct AstNode* CallNode)
{
  printf("\n\n");
  struct IntermediateRepresentationOp Call;

  Call.Opcode = CALL;
  Call.ParametersLength = 0;
  Call.Parameters = malloc(0);

  //Iterate through parameters and create values for them
  for (int i = 0; i < CallNode->ParameterLength; i++)
  {
    //Expand array
    struct IntermediateRepresentationValue** Temp = realloc(Call.Parameters, (Call.ParametersLength + 1) * sizeof(struct IntermediateRepresentationValue*));

    if (Temp == NULL)
    {
      free(Call.Parameters);
      return Call;
    }

    Call.ParametersLength++;

    struct IntermediateRepresentationValue* Value = malloc(sizeof(struct IntermediateRepresentationValue*));

    if (Value == NULL)
    {
      free(Call.Parameters);
      return Call;
    }

    *Value = GetValue(CallNode->Parameters[i]);

    Call.Parameters = Temp;
    Call.Parameters[i] = Value;
  }

  return Call;
}

void FreeIR(struct IntermediateRepresentationOp* OPs, int Size)
{
  for (int i = 0; i < Size; i++)
  {
    if (OPs[i].ParametersLength > 0)
    {
      for (int j = 0; j < OPs[i].ParametersLength; j++)
      {
        if (OPs[i].Parameters[j]->T == IR_STRING || OPs[i].Parameters[j]->T == IR_IDENT)
          free(OPs[i].Parameters[j]->V.s);

        free(OPs[i].Parameters[j]);
      }

      free(OPs[i].Parameters);
    }
  }

  free(OPs);
}

struct IntermediateRepresentationOp* GenerateIR(struct AST* Tree, int* Outsize)
{
  struct IntermediateRepresentationOp* Opcodes = malloc(0);
  int Size = 0;

  struct IntermediateRepresentationOp* Temp = realloc(Opcodes, (Size + 1) * sizeof(struct IntermediateRepresentationOp));

  if (Temp == NULL)
  {
    free(Opcodes);
    return NULL;
  }

  struct IntermediateRepresentationOp op = GenerateIR_FunctionDef(Tree);
  Opcodes = Temp;
  Opcodes[Size] = op;
  Size++;

  for (int i = 0; i < Tree->NodeLength; i++)
  {
    switch (Tree->Nodes[i]->Type)
    {
      case FunctionCall:
      {
        Temp = realloc(Opcodes, (Size + 1) * sizeof(struct IntermediateRepresentationOp));

        if (Temp == NULL)
        {
          free(Opcodes);
          return NULL;
        }

        op = GenerateIR_FunctionCall(Tree, Tree->Nodes[i]);
        Opcodes = Temp;
        Opcodes[Size] = op;
        Size++;
      }
      break;

      case Using:
      {
        Temp = realloc(Opcodes, (Size + 1) * sizeof(struct IntermediateRepresentationOp));

        if (Temp == NULL)
        {
          free(Opcodes);
          return NULL;
        }

        op = GenerateIR_Use(Tree->Nodes[i]);
        Opcodes = Temp;
        Opcodes[Size] = op;
        Size++;
      }
      break;

      default:
        printf("Default\n");
        break;
    }
  }

  *Outsize = Size;

  return Opcodes;
}
