#include "raylib.h"
#include <string>
#include "wavepool/vec2.cpp"
#include "wavepool/ripple_spawner.cpp"

using namespace wavepool;

int main(void)
{
    // Initialization
    vec2 screenSize = vec2(800, 800);
    InitWindow(screenSize.x, screenSize.y, "raylib cpp test");
    SetTargetFPS(60);

    WavePool wavePool = WavePool(screenSize, 20, 48, 48, 6, LIME);

    auto clickRippleParameters = RippleParameters(10, 6, 200, 30, 1, 3);
    auto centreRippleParameters = RippleParameters(50, 30, 200, 50, 5, 5);
    auto heldRippleParameters = RippleParameters(10, 3, 200, 20, 1, 1.5);
    RippleSpawner rippleSpawner = RippleSpawner(&wavePool, clickRippleParameters, centreRippleParameters,
        heldRippleParameters, screenSize, 30);

    // Main Game Loop
    while(!WindowShouldClose())
    {
        auto mousePosition = GetMousePosition();

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            rippleSpawner.SpawnRipple(mousePosition);
        else if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            rippleSpawner.SpawnHeldRipple(mousePosition);
        
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