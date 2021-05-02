#pragma once
#include <Assembler.h>

int Asm_GenerateFunctionCall(struct IntermediateRepresentationOp* Opcodes, int Index, int Size, char* Buffer);
int Asm_GenerateFunctionDef(struct IntermediateRepresentationOp* Opcodes, int Index, int Size, char* Buffer);
