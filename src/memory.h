#pragma once
#include "common.h"

#define new(X) (X*)M_MemAlloc(sizeof(X))

void assert(bool cond);

void* M_MemAlloc(uint size);
void* M_MemFree(void* ptr);

void CheckAllocations();
