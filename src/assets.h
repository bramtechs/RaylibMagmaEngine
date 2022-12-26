#pragma once

#include <stdio.h>

#include "raylib.h"
#include "raymath.h"

#include "logger.h"
#include "memory.h"
#include "array.h"

#define MAX_ASSETS 512 

typedef struct {
    char folder[128];

    Array* textures;
    Array* texturePaths;

} GameAssets;

void InitAssets(const char* folder);
void DisposeAssets();

Texture RequestTexture(const char* name);
Model RequestModel(const char* name);

FilePathList IndexModels(const char* folder);
