#pragma once

#include <stdio.h>

#include "raylib.h"
#include "raymath.h"

#include "logger.h"
#include "memory.h"

#define MAX_ASSETS 1000

// TODO to complicated!!!!!
// just having an array of all asset types will be the easiest solution tbh

typedef enum {
    AssetTexture,
    AssetImage,
    AssetMesh,
    AssetModel,
    AssetSound,
} AssetType; // TODO don't keep this

typedef struct {
    AssetType type;
    char name[256]; // TODO don't keep this
    void* memory;
} Asset;

typedef struct {
    char folder[256];
    Asset assets[MAX_ASSETS];
    size_t count;
} Assets;

typedef struct {
    const char** names;
    size_t count; 
} AssetList;

// TODO try to use macros

void LoadAssets(const char* folder);
void UnloadAssets();

Model* GetModel(const char* name);

AssetList GetLoadedAssetList();
void UnloadAssetList(AssetList list);

