#include "main_menu.h"

typedef struct {
    bool skipSplash;
    Color saveCol;
} MainMenuSession;

static MainMenuSession MenuSession = { 0 };

// TODO add dispose
static MainMenuConfig* MenuConfig = NULL;

void BootMainMenu(MainMenuConfig config, bool skipSplash){
    // TODO hack
    assert(MenuConfig == NULL);
    MenuConfig = new(MainMenuConfig);
    memcpy(MenuConfig, &config, sizeof(MainMenuConfig));

    // initialize session
    MenuSession.skipSplash = skipSplash;
    MenuSession.saveCol = WHITE;

    INFO("Booting main menu!");
}

void DrawScreenSaver(float delta){
    ClearBackground(MenuSession.saveCol);
    Color* c = &MenuSession.saveCol;
    c->r += delta*10.f;
    c->g -= delta*10.f;
    c->b += delta*20.f;
    c->a = 255;
}

bool UpdateAndDrawMainMenu(float delta) {
    BeginMagmaDrawing();

    DrawScreenSaver(delta);

    EndMagmaDrawing();
    EndDrawing();
    return false;
}
