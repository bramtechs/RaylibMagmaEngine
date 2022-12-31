#include "main_menu.h"

typedef struct {
    bool skipSplash;
    Color saveCol;

    Texture splashTextures[MAX_SPLASHES];
    Texture bgTexture;
} MainMenuSession;

static MainMenuSession MenuSession = { 0 };
static MainMenuConfig MenuConfig = { 0 };

void BootMainMenu(MainMenuConfig config, bool skipSplash){

    memcpy(&MenuConfig, &config, sizeof(MainMenuConfig));

    // initialize session
    MainMenuSession* mses = &MenuSession;
    mses->skipSplash = skipSplash;
    mses->saveCol = WHITE;

    // load each texture
    mses->bgTexture = RequestTexture(config.bgPath);
    
    for (int i = 0; i < config.splashCount; i++){
        mses->splashTextures[i] = RequestTexture(config.splashes[i].imgPath);
    }

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

// TODO make a more flexible/non-hardcoded implemenation
bool UpdateAndDrawMainMenu(float delta) {
    if (MenuConfig.width == 0){ // skip if not booted
        return true;
    }

    BeginMagmaDrawing();

    BeginCouroutine();

    for (int i = 0; i < MenuConfig.splashCount; i++){
        SplashScreen splash = MenuConfig.splashes[i];
        Texture texture = MenuSession.splashTextures[i];

        float light = MIN(sqrt(sinf(CTIMER*0.5*PI-1.6)+1)*1.5f,1.f);
        unsigned char lightByte = light*255;
        Color tint = {lightByte, lightByte, lightByte, 255};

        // TODO make stretch to the entire window
        DrawTexture(texture, 0, 0, tint);
        DrawText(TextFormat("%f",light),50,50,72,RED);

        SleepCouroutine(4);
    }
    DrawTexture(MenuSession.bgTexture, 0, 0, WHITE);

    EndCouroutine();

    EndMagmaDrawing();
    EndDrawing();

    return false;
}
