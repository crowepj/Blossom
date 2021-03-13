#include "DynamicArray.h"
#include <stdio.h>
#include <stdlib.h>

int DynamicArray_Initialize(struct DynamicArray* This, int TypeSize)
{
	This->Begin = malloc(TypeSize);

	if (This->Begin == NULL) return 0;

	This->Length = 0;
	This->Allocated = TypeSize;

	return 1;
}

int DynamicArray_PushBack(struct DynamicArray* This, void* Value)
{
	//One size of one entry is the total size of the array in bytes / the amount of entries in the array, i.e. An array has 10 entries, the array is 100 bytes long, 100 / 10 = 10, one entry is 10 bytes

	//If it's the first element, just set the value, you don't have to allocate any more space
	if (This->Length == 0)
	{
		This->Length = 1;
		*This->Begin = Value;
		return 1;
	}

	int TypeSize = (This->Allocated / This->Length);

	void** Loc = realloc(This->Begin, This->Allocated + TypeSize);

	if (Loc == NULL)
		return 0;

	This->Begin = Loc;
	This->Length++;

	*(This->Begin + This->Allocated) = Value;

	This->Allocated += TypeSize;

	return 1;
}

void* DynamicArray_At(struct DynamicArray* This, int index)
{
	return *(This->Begin + (This->Allocated / This->Length) * index);
}

void DynamicArray_Free(struct DynamicArray* This)
{
	free(This->Begin);
}
