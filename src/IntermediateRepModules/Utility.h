#pragma once
#include <IntermediateRepresentation.h>

struct IntermediateRepresentationValue GetValue(AstValue Value);
unsigned char AppendParameter(struct IntermediateRepresentationOp* op, struct IntermediateRepresentationValue* value);
struct IntermediateRepresentationValue* MakeValuePointer(struct IntermediateRepresentationValue val);
