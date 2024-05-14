#include "raylib.h"
#include <string>
#include "wavepool/vec2.cpp"
#include "wavepool/wave_pool.cpp"

using namespace wavepool;

int main(void)
{
    // Initialization
    vec2 screenSize = vec2(800, 450);
    InitWindow(screenSize.x, screenSize.y, "raylib cpp test");
    SetTargetFPS(60);

    WavePool wavePool = WavePool(screenSize, 20, 32, 18, 8, LIME);

    // Main Game Loop
    while(!WindowShouldClose())
    {
        auto mousePosition = GetMousePosition();
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            wavePool.AddRipple(Ripple(vec2(mousePosition), 5));
        
        wavePool.Update();

        BeginDrawing();

        ClearBackground(DARKGRAY);
        wavePool.Draw();
        DrawText(std::to_string(1 / GetFrameTime()).c_str(), 5, 5, 24, BLACK);

        EndDrawing();
    }
    // De-Initialization
    CloseWindow();
    return 0;
}