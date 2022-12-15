#pragma once

#include <stdio.h>

#include "raylib.h"
#include "raymath.h"

#include "logger.h"
#include "memory.h"

#define MAX_ASSETS 512 

typedef struct {
	char folder[128];

	Texture textures[MAX_ASSETS];
	size_t textureCount;
	Model models[MAX_ASSETS];
	size_t modelCount;

} Assets;


void InitAssets(const char* folder);
void UnloadAssets();

Texture RequestTexture(const char* name);
Model RequestModel(const char* name);
