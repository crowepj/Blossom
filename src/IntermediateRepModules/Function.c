#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Function.h"
#include "Utility.h"

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
