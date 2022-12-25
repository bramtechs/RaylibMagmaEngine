#pragma once

#include <stdio.h>
#include <stdbool.h>
#include "logger.h"
#include "memory.h"

// A serializable list that is contained in a single block. It uses relative pointers to allow navigation.

// HEADER 1 (ListItem)
//      / RAW <--- begin data block
//      | RAW
//      | RAW
// size | RAW
//      | RAW
//      \ RAW <--- end data block
// HEADER 2 (ListItem)
//      / RAW <--- begin data block
//      | RAW
//  ......etc....

typedef unsigned int ItemType;

typedef struct {
    size_t size;
    ItemType type;
} ListItem;

typedef struct {
    char* data;
    size_t capacity; // in bytes
    size_t size; // in bytes
    size_t count; // number of items 
} List;

typedef struct {
    List* list;
    ListItem* current;
    size_t curIndex;
} ListIterator;

List* MakeList();
void DisposeList(List* list);
void PushListRaw(List* list, void* data, size_t size, ItemType type);

ListIterator IterateListItems(List* list);
bool IterateNextItem(ListIterator* it, ItemType *type, void* result);

List* ImportList(const char* fileName);
void ExportList(List* list, const char* fileName);

void TestList();
