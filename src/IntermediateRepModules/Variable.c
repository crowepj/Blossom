#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Utility.h"
#include "Variable.h"

struct IntermediateRepresentationOp GenerateIR_Variable_Constant_Assign(struct AstNode* Node)
{
  struct IntermediateRepresentationOp Assign;
  Assign.Opcode = V_ASSIGN;
  Assign.Children = malloc(0);
  Assign.ChildrenLength = 0;

  struct IntermediateRepresentationValue name = GetValue(Node->Children[0]->Value);

  if (name.T != IR_IDENT && name.T != IR_STRING)
    return;

  AppendParameter(&Assign, name);

  for (int i = 1; i < Node->ChildrenLength; i++)
  {
    struct IntermediateRepresentationValue value = GetValue(Node->Children[i]->Value);

    AppendParameter(&Assign, value);
  }

  return Assign;
}
