#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Utility.h"

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

unsigned char AppendParameter(struct IntermediateRepresentationOp* op, struct IntermediateRepresentationValue value)
{
  struct IntermediateRepresentationOp** temp_arr = realloc(op->Children, (op->ChildrenLength + 1) * sizeof(struct IntermediateRepresentationOp*));

  if (!temp_arr)
    return 1;

  struct IntermediateRepresentationOp* op_add = malloc(sizeof(struct IntermediateRepresentationOp));

  if (!op)
    return 1;

  op_add->Opcode = IR_VALUE;
  op_add->Value = value;

  op->Children = temp_arr;
  op->Children[op->ChildrenLength] = op_add;
  op->ChildrenLength++;

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
