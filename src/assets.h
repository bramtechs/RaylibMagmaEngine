#pragma once

#include <stdio.h>

#include "raylib.h"
#include "raymath.h"

#include "logger.h"
#include "memory.h"

#define MAX_ASSETS 1000

// TODO too complicated!!!!!
// just having an array of all asset types will be the easiest solution tbh

typedef struct {
    char folder[256];
} Assets;

typedef struct {
    const char** names;
    size_t count; 
} AssetList;

// TODO try to use macros

void LoadAssets(const char* folder);
void UnloadAssets();

Model RequestModel(const char* name);
Texture RequestTexture(const char* name);
