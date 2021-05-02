#include "IntermediateRepresentation.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <IntermediateRepModules/Function.h>
#include <IntermediateRepModules/Utility.h>
#include <IntermediateRepModules/Variable.h>

struct IntermediateRepresentationOp GenerateIR_Use(struct AstNode* Node)
{

};

void FreeIR(struct IntermediateRepresentationOp* OPs, int Size)
{
  for (int i = 0; i < Size; i++)
  {
    if (OPs[i].ChildrenLength > 0)
    {
      for (int j = 0; j < OPs[i].ChildrenLength; j++)
      {
        if (OPs[i].Children[j]->Value.T == IR_STRING || OPs[i].Children[j]->Value.T == IR_IDENT)
          free(OPs[i].Children[j]->Value.V.s);

        free(OPs[i].Children[j]);
      }

      free(OPs[i].Children);
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

      case VariableDefinition:
      {
        Temp = realloc(Opcodes, (Size + 1) * sizeof(struct IntermediateRepresentationOp));

        if (Temp == NULL)
        {
          free(Opcodes);
          return NULL;
        }

        op = GenerateIR_Variable_Constant_Assign(Tree->Nodes[i]);
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
