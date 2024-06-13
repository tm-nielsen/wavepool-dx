#include "raylib.h"
#include <string>
#include "utils/vec2.cpp"
#include "wavepool/radial_instrument.cpp"
#include "settings/settings.cpp"

using namespace wavepool;
using namespace settings;
using namespace utils;

int main(void)
{
    // Initialization
    Settings settings = Settings::LoadFromFile();
    vec2 screenSize = vec2(settings.windowWidth, settings.windowHeight);
    float margin = settings.margin;

    InitWindow(screenSize.x, screenSize.y, "raylib cpp test");
    SetWindowMinSize(500, 500);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    InitAudioDevice();
    SetMasterVolume(settings.volume);
    SetTargetFPS(60);

    WavePool wavePool = WavePool(screenSize, margin,
        settings.dotSize, settings.dotSpacing, settings.mainColour);

    auto clickRippleParameters = RippleParameters(10, 6, 200, 30, 1, 3);
    auto centreRippleParameters = RippleParameters(20, 30, 200, 40, 5, 5);
    auto heldRippleParameters = RippleParameters(10, 3, 200, 20, 1, 1.5);
    RippleSpawner rippleSpawner = RippleSpawner(&wavePool, clickRippleParameters,
        centreRippleParameters, heldRippleParameters, 30);

    RadialInstrument radialInstrument= RadialInstrument(&rippleSpawner, screenSize, 100, margin);
    radialInstrument.LoadSounds();
    

    // Main Game Loop
    while(!WindowShouldClose())
    {
        if (IsWindowResized())
        {
            vec2 newScreenSize = vec2(GetScreenWidth(), GetScreenHeight());
            radialInstrument.OnWindowResized(newScreenSize);
            wavePool.OnWindowResized(newScreenSize);
        }

        radialInstrument.Update();
        wavePool.Update();

        BeginDrawing();

        ClearBackground(settings.backgroundColour);
        radialInstrument.DrawGuides(settings.guideColour, 6);
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