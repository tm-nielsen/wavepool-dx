#include "raylib.h"
#include "wave_pool/radial_instrument.cpp"
#include "window_management/window_manager.cpp"
#include "settings/settings_menu.cpp"

using namespace window_management;
using namespace wave_pool;
using namespace settings;
using namespace utils;

int main(void)
{
    // Initialization
    Settings settings = Settings::LoadFromFile();
    vec2 screenSize = vec2(settings.windowWidth, settings.windowHeight);
    float margin = settings.margin;

    Color mainColour = settings.mainColour;
    Color backgroundColour = settings.backgroundColour;
    Color accentColour = settings.accentColour;
    float lineThickness = settings.lineThickness;


    SetConfigFlags(FLAG_WINDOW_TRANSPARENT);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetConfigFlags(FLAG_WINDOW_UNDECORATED);
    InitWindow(screenSize.x, screenSize.y, "Wave Pool");
    SetTargetFPS(60);
    InitAudioDevice();
    SetMasterVolume(settings.volume);


    rect backgroundRect = rect(margin, screenSize - (2 * margin));

    WindowManager windowManager = WindowManager(vec2(500));
    windowManager.LoadResources();
    windowManager.SetStyle(accentColour, mainColour, lineThickness);
    windowManager.OnWindowResized(screenSize, margin);

    SettingsMenu settingsMenu = SettingsMenu(&settings);
    settingsMenu.LoadResources();
    settingsMenu.SetStyle(accentColour, mainColour, lineThickness);
    settingsMenu.OnWindowResized(screenSize, margin);

    WavePool wavePool = WavePool(screenSize, margin,
        settings.dotSize, settings.dotSpacing, mainColour);

    RippleSpawner rippleSpawner = RippleSpawner(&wavePool, 30);

    RadialInstrument radialInstrument= RadialInstrument(&rippleSpawner, screenSize, margin);
    radialInstrument.LoadSounds();


    // Main Game Loop
    while(!WindowShouldClose())
    {
        if (windowManager.windowResized || settingsMenu.windowResized || settingsMenu.marginSettingModified)
        {
            screenSize = vec2(GetScreenWidth(), GetScreenHeight());
            margin = settings.margin;
            if (windowManager.windowResized)
            {
                settings.windowWidth = screenSize.x;
                settings.windowHeight = screenSize.y;
                settings.SaveToFile();
            }
            backgroundRect = rect(margin, screenSize - (2 * margin)).ToRectangle();
            windowManager.OnWindowResized(screenSize, margin);
            settingsMenu.OnWindowResized(screenSize, margin);
            radialInstrument.OnWindowResized(screenSize, margin);
            wavePool.OnWindowResized(screenSize, margin);
        }

        if (settingsMenu.styleSettingsModifed)
        {
            mainColour = settings.mainColour;
            backgroundColour = settings.backgroundColour;
            accentColour = settings.accentColour;
            lineThickness = settings.lineThickness;

            windowManager.SetStyle(accentColour, mainColour, lineThickness);
            windowManager.OnWindowResized(screenSize, margin);
            settingsMenu.SetStyle(accentColour, mainColour, lineThickness);
            settingsMenu.OnWindowResized(screenSize, margin);
            wavePool.SetColour(mainColour);
        }
        if (settingsMenu.waveGridSettingsModified)
        {
            wavePool.SetGridLayout(settings.dotSize, settings.dotSpacing);
        }

        windowManager.Update();
        if (windowManager.shouldExitProgram)
            break;
        
        if (windowManager.settingsMenuIsOpen)
        {
            settingsMenu.Update();
        }
        else
        {
            radialInstrument.Update();
            wavePool.Update();
        }

        BeginDrawing();

        ClearBackground(BLANK);
        backgroundRect.DrawRoundedFilled(backgroundColour, 0.075);

        windowManager.Draw();
        if (windowManager.settingsMenuIsOpen)
        {
            settingsMenu.Draw();
        }
        else
        {
            radialInstrument.DrawGuides(accentColour, lineThickness);
            wavePool.Draw();
        }
        if (settings.showFps)
            DrawText(std::to_string(1 / GetFrameTime()).c_str(), 5, 5, 24, BLACK);

        EndDrawing();
    }
    // De-Initialization
    windowManager.UnloadResources();
    settingsMenu.UnloadResources();
    radialInstrument.UnloadSounds();
    CloseAudioDevice();
    CloseWindow();
    return 0;
}