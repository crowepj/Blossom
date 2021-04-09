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

    //If type is a string we need to duplicate it to avoid double frees
    if (l_Value.T == IR_STRING || l_Value.T == IR_IDENT)
    {
      l_Value.V.s = strdup(Value.Value.s);
      return l_Value;
    }

    /*
    //Keep this here in case something bad happens
    switch (Value.Type)
    {
      case AST_INTEGER:
        l_Value.V.i = Value.Value.i;
        break;
      case AST_IDENTIFIER:
      case AST_STRING:
        //Duplicate so that this stage doesn't have a dependency on the AST being not freed
        l_Value.V.s = strdup(Value.Value.s);
        break;
      case AST_FLOAT:
        l_Value.V.f = Value.Value.f;
        break;
    }
    */

    l_Value.V = *((union _Value*)&Value.Value);

    return l_Value;
  }

  struct IntermediateRepresentationValue err;
  err.T = IR_VAL_ERR;

  return err;
}

unsigned char AppendParameter(struct IntermediateRepresentationOp* op, struct IntermediateRepresentationValue* value)
{
  struct IntermediateRepresentationValue** temp_arr = realloc(op->Parameters, (op->ParametersLength + 1) * sizeof(struct IntermediateRepresentationValue*));

  if (!temp_arr)
    return 1;

  op->Parameters = temp_arr;
  op->Parameters[op->ParametersLength] = value;
  op->ParametersLength++;

  return 0;
}

struct IntermediateRepresentationValue* MakeValuePointer(struct IntermediateRepresentationValue val)
{
  struct IntermediateRepresentationValue* temp = malloc(sizeof(struct IntermediateRepresentationValue));

  if (!temp)
    return NULL;

  *temp = val;
  return temp;
};

struct IntermediateRepresentationOp GenerateIR_FunctionDef(struct AST* Tree)
{
  struct IntermediateRepresentationOp opcode;
  opcode.Opcode = FUNCDEF;
  opcode.Parameters = malloc(0);

  struct IntermediateRepresentationValue value;
  value.T = IR_IDENT;
  value.V.s = strdup(Tree->Name);

  struct IntermediateRepresentationValue* valuePointer = MakeValuePointer(value);

  if (!valuePointer)
    return;

  AppendParameter(&opcode, valuePointer);

  return opcode;
}

struct IntermediateRepresentationOp GenerateIR_Use(struct AstNode* Node)
{

};

struct IntermediateRepresentationOp GenerateIR_FunctionCall(struct AST* Tree, struct AstNode* CallNode)
{
  struct IntermediateRepresentationOp INS;
  INS.Opcode = CALL;
  INS.Parameters = malloc(0);
  INS.ParametersLength = 0;

  for (int i = 0; i < CallNode->ChildrenLength; i++)
  {
    struct IntermediateRepresentationValue Value = GetValue(CallNode->Children[i]->Value);

    if (Value.T == IR_VAL_ERR)
      return;

    struct IntermediateRepresentationValue* Pointerized = MakeValuePointer(Value);

    //TODO Return some value to signify failure
    if (!Pointerized)
    {
      return;
    }

    AppendParameter(&INS, Pointerized);
  }

  return INS;
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
        //realloc was successful, set it
        Opcodes = Temp;
        //set opcode
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
        //realloc was successful, set it
        Opcodes = Temp;
        //set opcode
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
