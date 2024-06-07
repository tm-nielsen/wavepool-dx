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

    WavePool wavePool = WavePool(screenSize, 20, 48, 27, 6, LIME);

    float ripple_spawn_delay = 1.0 / 30;
    float ripple_spawn_delay_timer = 0;

    // Main Game Loop
    while(!WindowShouldClose())
    {
        auto mousePosition = GetMousePosition();

        ripple_spawn_delay_timer += GetFrameTime();
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            if (ripple_spawn_delay_timer >= ripple_spawn_delay)
            {
                ripple_spawn_delay_timer -= ripple_spawn_delay;
                wavePool.AddRipple(Ripple(vec2(mousePosition), 10));
            }
        }
        else if (ripple_spawn_delay_timer > ripple_spawn_delay)
            ripple_spawn_delay_timer = ripple_spawn_delay;
        
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