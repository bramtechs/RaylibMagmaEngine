#pragma once

#include "raylib.h"
#include "raymath.h"
#include "logger.h"
#include "window.h"
#include <string.h> 

#define FADE_DURATION 2

typedef struct {
    const char imgPath[128];
    float duration;
} SplashScreen;

typedef struct {
    int width;
    int height;

    SplashScreen splashes[8];
    size_t splashCount;

    const char bgPath[128]; // background image

} MainMenuConfig;

void BootMainMenu(MainMenuConfig config, bool skipSplash);
bool UpdateAndDrawMainMenu(float delta); // returns 'true' if done
