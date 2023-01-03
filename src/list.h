#pragma once

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <map>
#include <iterator>
#include <assert.h>

#include "logger.h"
#include "memory.h"

// this code did not port easily

typedef int ItemType;

struct List : public std::multimap<ItemType,void*> { // lol
    ~List();

    template <typename T>
    void Push(ItemType type, T& data);
};

void ImportList(List* list, const char* fileName);
void ExportList(List* list, const char* fileName);

void TestList();
