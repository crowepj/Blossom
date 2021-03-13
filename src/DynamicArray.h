#pragma once
#include "Tokens.h"

struct DynamicArray 
{
	void** Begin;
	int Length;
	int Allocated;
};

int DynamicArray_Initialize(struct DynamicArray* This, int TypeSize);
int DynamicArray_PushBack(struct DynamicArray* This, void* Value);
void* DynamicArray_At(struct DynamicArray* This, int index);
void DynamicArray_Free(struct DynamicArray* This);