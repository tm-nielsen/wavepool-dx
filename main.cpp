#include "raylib.h"
#include <string>
#include "utils/vec2.cpp"
#include "wavepool/radial_instrument.cpp"
#include "settings/settings_menu.cpp"
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


    SetConfigFlags(FLAG_WINDOW_TRANSPARENT);
    InitWindow(screenSize.x, screenSize.y, "Wave Pool");
    SetWindowMinSize(500, 500);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetWindowState(FLAG_WINDOW_UNDECORATED);
    InitAudioDevice();
    SetMasterVolume(settings.volume);
    SetTargetFPS(60);


    Rectangle backgroundRect = rect(margin, screenSize - (2 * margin)).ToRectangle();

    SettingsMenu settingsMenu = SettingsMenu(screenSize, margin);
    settingsMenu.LoadResources();
    settingsMenu.SetStyle(settings.guideColour, settings.mainColour, 6);

    WavePool wavePool = WavePool(screenSize, margin,
        settings.dotSize, settings.dotSpacing, settings.mainColour);

    RippleSpawner rippleSpawner = RippleSpawner(&wavePool, 30);

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

        settingsMenu.Update();
        if (!settingsMenu.isOpen)
        {
            radialInstrument.Update();
            wavePool.Update();
        }

        BeginDrawing();

        ClearBackground(BLANK);
        DrawRectangleRounded(backgroundRect, 0.075, 6, settings.backgroundColour);

        settingsMenu.Draw();
        if (!settingsMenu.isOpen)
        {
            radialInstrument.DrawGuides(settings.guideColour, 6);
            wavePool.Draw();
            DrawText(std::to_string(1 / GetFrameTime()).c_str(), 5, 5, 24, BLACK);
        }

        EndDrawing();
    }
    // De-Initialization
    settingsMenu.UnloadResources();
    radialInstrument.UnloadSounds();
    CloseAudioDevice();
    CloseWindow();
    return 0;
}