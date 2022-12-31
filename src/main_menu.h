#pragma once

#include "raylib.h"
#include "raymath.h"
#include "logger.h"
#include "window.h"
#include "assets.h"
#include "couroutines.h"
#include <string.h> 

#define MAX_SPLASHES 8
#define FADE_DURATION 2

typedef struct {
    const char imgPath[128];
} SplashScreen;

typedef struct {
    int width;
    int height;

    SplashScreen splashes[MAX_SPLASHES];
    size_t splashCount;

    const char bgPath[128]; // background image
} MainMenuConfig;

void BootMainMenu(MainMenuConfig config, bool skipSplash);
bool UpdateAndDrawMainMenu(float delta); // returns 'true' if done
