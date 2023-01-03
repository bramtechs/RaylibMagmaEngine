#pragma once

#include <stdio.h>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>

#include "raylib.h"
#include "raymath.h"

#include "logger.h"
#include "memory.h"

#define MAX_ASSETS 512 

struct GameAssets {
    std::string folder;
    std::unordered_map<std::string, Texture> textures;
};

void InitAssets(const char* folder);
void DisposeAssets();

const char* GetAssetFolder();

Texture RequestTexture(const char* name);
Model RequestModel(const char* name);
Shader RequestShader(const char* name);

FilePathList IndexModels(const char* folder);
