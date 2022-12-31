#include "main_menu.h"

typedef struct {
    bool skipSplash;
    Color saveCol;

    Texture splashTextures[MAX_SPLASHES];
    Texture bgTexture;
} MainMenuSession;

static MainMenuSession MenuSession = { 0 };
static MainMenuConfig MenuConfig = { 0 };

// couroutines.h (HIGHLY EXPERIMENTAL)
// embrace the goto jank

#define BeginCouroutine() \
    static float _target = 0.f; \
    static float _timer = 0.f; \
    _timer += delta

#define SleepCouroutine(X) \
    _target += X; \
    if (_timer < _target) goto cour_end

#define RestartCouroutine() \
    _timer = 0; \
    goto cour_end

#define EndCouroutine() cour_end: _target = 0.f

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

bool UpdateAndDrawMainMenu(float delta) {
    BeginMagmaDrawing();

    BeginCouroutine();
    INFO("%f",_timer);

    for (int i = 0; i < MenuConfig.splashCount; i++){
        SplashScreen splash = MenuConfig.splashes[i];
        Texture texture = MenuSession.splashTextures[i];

        // TODO make stretch to the entire window
        DrawTexture(texture, 0, 0, WHITE);

        SleepCouroutine(splash.duration);
    }
    DrawTexture(MenuSession.bgTexture, 0, 0, WHITE);

    EndCouroutine();

    EndMagmaDrawing();
    EndDrawing();

    return false;
}
