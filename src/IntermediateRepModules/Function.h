#pragma once
#include <IntermediateRepresentation.h>

struct IntermediateRepresentationOp GenerateIR_FunctionDef(struct AST* Tree);
struct IntermediateRepresentationOp GenerateIR_FunctionCall(struct AST* Tree, struct AstNode* CallNode);
