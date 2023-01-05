#include "magma.h"

int main() {
    // Initialization
    //--------------------------------------------------------------------------------------
    int screenWidth = 800;
    int screenHeight = 450;
    raylib::Color textColor = raylib::Color::LightGray();
    raylib::Window window(screenWidth, screenHeight, "magma [core] example - basic window");

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!window.ShouldClose()) {   // Detect window close button or ESC key
        // Update
        //----------------------------------------------------------------------------------
        // Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        {
            window.ClearBackground(ORANGE);
            textColor.DrawText("Congrats! You created your magma window!", 190, 200, 20);
        }
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    return 0;
}
