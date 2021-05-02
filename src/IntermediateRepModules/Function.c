#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Function.h"
#include "Utility.h"

struct IntermediateRepresentationOp GenerateIR_FunctionDef(struct AST* Tree)
{
  struct IntermediateRepresentationOp opcode;
  opcode.Opcode = FUNCDEF;
  opcode.Children = malloc(0);
  opcode.ChildrenLength = 0;

  struct IntermediateRepresentationValue value;
  value.T = IR_IDENT;
  value.V.s = strdup(Tree->Name);

  AppendParameter(&opcode, value);

  return opcode;
}

struct IntermediateRepresentationOp GenerateIR_FunctionCall(struct AST* Tree, struct AstNode* CallNode)
{
  struct IntermediateRepresentationOp INS;
  INS.Opcode = CALL;
  INS.Children = malloc(0);
  INS.ChildrenLength = 0;

  for (int i = 0; i < CallNode->ChildrenLength; i++)
  {
    struct IntermediateRepresentationValue Value = GetValue(CallNode->Children[i]->Value);

    if (Value.T == IR_VAL_ERR)
      return;

    AppendParameter(&INS, Value);
  }

  return INS;
}
