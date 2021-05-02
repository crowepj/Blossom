#pragma once
#include "Parser.h"

enum IntermediateRepresentationType
{
  IR_INT,
  IR_FLOAT,
  IR_STRING,
  IR_CHAR,
  IR_IDENT,
  IR_STRUCT,

  //Used to signify failure, e.g. couldn't convert a type
  IR_VAL_ERR
};

struct IntermediateRepresentationValue
{
  enum IntermediateRepresentationType T;
  union _Value
  {
    int i;
		float f;
		char* s;
		char c;
  } V;
};

enum IntermediateRepresentationInstruction
{
  ADD,
  SUB,
  MUL,
  DIV,
  CALL,
  V_ASSIGN,
  FUNCDEF,
  IR_USE,
  IR_VALUE,
  //Err opcode means an error occured in generating the IR, e.g. malloc failed
  ERR,
};

struct IntermediateRepresentationOp
{
  enum IntermediateRepresentationInstruction Opcode;
  struct IntermediateRepresentationOp** Children;
  struct IntermediateRepresentationValue Value;
  unsigned int ChildrenLength;
};

void FreeIR(struct IntermediateRepresentationOp* OPs, int Size);
struct IntermediateRepresentationOp* GenerateIR(struct AST* Tree, int* OutSize);
