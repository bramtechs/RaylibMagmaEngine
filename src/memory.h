#pragma once

extern size_t Allocations;

void* M_MemAlloc(size_t size);
void* M_MemFree(void* ptr);

void CheckAllocations();
