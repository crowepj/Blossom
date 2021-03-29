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
  PUSH,
  POP,
  ADD,
  SUB,
  MUL,
  DIV,
  CALL,
  FUNCDEF,
  IR_USE,
};

struct IntermediateRepresentationOp
{
  enum IntermediateRepresentationInstruction Opcode;
  struct IntermediateRepresentationValue** Parameters;
  unsigned int ParametersLength;
};

void FreeIR(struct IntermediateRepresentationOp* OPs, int Size);
struct IntermediateRepresentationOp* GenerateIR(struct AST* Tree, int* OutSize);
