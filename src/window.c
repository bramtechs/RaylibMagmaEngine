#include "window.h"

// adapted from core_window_letterbox, thank you ~!

// TODO put in struct!
static int GameScreenWidth = 0;
static int GameScreenHeight = 0;
static int WindowWidth = 0;
static int WindowHeight = 0;
static RenderTexture2D RenderTarget = { 0 };

float GetMagmaScaleFactor(){
    return MIN((float)GetScreenWidth()/GameScreenWidth, (float)GetScreenHeight()/GameScreenHeight);
}

float GetLeftMagmaWindowOffset(){
    float scale = GetMagmaScaleFactor(); // TODO cache this in global variable
    return (GetScreenWidth() - ((float)GameScreenWidth*scale))*0.5f;
}
float GetTopMagmaWindowOffset(){
    float scale = GetMagmaScaleFactor();
    return (GetScreenHeight() - ((float)GameScreenHeight*scale))*0.5f;
}

void InitMagmaWindow(int gameWidth, int gameHeight, int winWidth, int winHeight, const char* title){

    GameScreenWidth = gameWidth;
    GameScreenHeight = gameHeight;
    WindowWidth = winWidth;
    WindowHeight = winHeight;

    assert(GameScreenWidth > 0 && GameScreenHeight > 0 &&
           WindowWidth > 0 && WindowHeight > 0);

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(GameScreenWidth,GameScreenHeight,title);
    SetWindowMinSize(WindowWidth/2,WindowHeight/2);
    //SetWindowPosition((GetMonitorWidth(0) - WIDTH * SCALE) / 2, (GetMonitorHeight(0) - HEIGHT * SCALE) / 2);

    // Render texture initialization, used to hold the rendering result so we can easily resize it
    RenderTarget = LoadRenderTexture(GameScreenWidth, GameScreenHeight);
    SetTextureFilter(RenderTarget.texture, TEXTURE_FILTER_POINT);  // Texture scale filter to use
}

void BeginMagmaDrawing(){
    float scale = GetMagmaScaleFactor();

    // Update virtual mouse (clamped mouse value behind game screen)
    Vector2 virtualMouse = GetScaledMousePosition();

    // Apply the same transformation as the virtual mouse to the real mouse (i.e. to work with raygui)
    //SetMouseOffset(-(GetScreenWidth() - (GameScreenWidth*scale))*0.5f, -(GetScreenHeight() - (GameScreenHeight*scale))*0.5f);
    //SetMouseScale(1/scale, 1/scale);
    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    // Draw everything in the render texture, note this will not be rendered on screen, yet
    BeginTextureMode(RenderTarget);
        ClearBackground(BLACK);  // Clear render texture background color

        // ....
}

void EndMagmaDrawing(){

    // ....
    float scale = GetMagmaScaleFactor();

    EndTextureMode();
    
    BeginDrawing();
        ClearBackground(BLACK);     // Clear screen background

        float left = GetLeftMagmaWindowOffset();
        float top = GetTopMagmaWindowOffset();

        // Draw render texture to screen, properly scaled
        DrawTexturePro(RenderTarget.texture, (Rectangle){ 0.0f, 0.0f, (float)RenderTarget.texture.width, (float)-RenderTarget.texture.height },
                       (Rectangle){ left, top, (float)GameScreenWidth*scale, (float)GameScreenHeight*scale }, (Vector2){ 0, 0 }, 0.0f, WHITE);
}

void CloseMagmaWindow(){
    UnloadRenderTexture(RenderTarget);
    CloseWindow();
}

Vector2 GetScaledMousePosition(){
        float scale = GetMagmaScaleFactor(); 

        // Update virtual mouse (clamped mouse value behind game screen)
        Vector2 mouse = GetMousePosition();
        Vector2 virtualMouse = { 0 };
        virtualMouse.x = (mouse.x - (GetScreenWidth() - (GameScreenWidth*scale))*0.5f)/scale;
        virtualMouse.y = (mouse.y - (GetScreenHeight() - (GameScreenHeight*scale))*0.5f)/scale;
        virtualMouse = Vector2Clamp(virtualMouse, (Vector2){ 0, 0 }, (Vector2){ (float)GameScreenWidth, (float)GameScreenHeight });
        return virtualMouse;
}
