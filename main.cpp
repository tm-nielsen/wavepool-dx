#include "raylib.h"
#include <string>
#include "wavepool/vec2.cpp"
#include "wavepool/radial_instrument.cpp"

using namespace wavepool;

int main(void)
{
    // Initialization
    vec2 screenSize = vec2(800, 800);
    float margin = 60;
    InitWindow(screenSize.x, screenSize.y, "raylib cpp test");
    SetTargetFPS(60);
    InitAudioDevice();

    WavePool wavePool = WavePool(screenSize, margin, 48, 48, 6, LIME);

    auto clickRippleParameters = RippleParameters(10, 6, 200, 30, 1, 3);
    auto centreRippleParameters = RippleParameters(20, 30, 200, 40, 5, 5);
    auto heldRippleParameters = RippleParameters(10, 3, 200, 20, 1, 1.5);
    RippleSpawner rippleSpawner = RippleSpawner(&wavePool, clickRippleParameters, centreRippleParameters,
        heldRippleParameters, screenSize, 30);

    RadialInstrument radialInstrument= RadialInstrument(&rippleSpawner, screenSize, 100, margin);
    radialInstrument.LoadSounds();
    

    // Main Game Loop
    while(!WindowShouldClose())
    {        
        radialInstrument.Update();
        wavePool.Update();

        BeginDrawing();

        ClearBackground(DARKGRAY);
        radialInstrument.DrawGuides(GRAY, 2);
        wavePool.Draw();
        DrawText(std::to_string(1 / GetFrameTime()).c_str(), 5, 5, 24, BLACK);

        EndDrawing();
    }
    // De-Initialization
    radialInstrument.UnloadSounds();
    CloseAudioDevice();
    CloseWindow();
    return 0;
}