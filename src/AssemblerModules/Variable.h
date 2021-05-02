#pragma once
#include <stddef.h>
#include <Assembler.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct _VariableType
{
  char* name;
  size_t stack_off;
  size_t size;
};

static struct _VariableType* Variables;
static size_t VariableLength = 0;
static size_t MostRecentStackOffset = 4;

int Asm_GenerateVariableConstant(struct IntermediateRepresentationOp* Opcodes, int Index, int Size, char* Buffer);
