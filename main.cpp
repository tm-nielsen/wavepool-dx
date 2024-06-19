#include "raylib.h"
#include "wave_pool/radial_instrument.cpp"

#ifndef PLATFORM_WEB
#include "window_management/window_manager.cpp"
#include "settings/settings_menu.cpp"
using namespace window_management;
using namespace settings;
#endif

using namespace wave_pool;

int main(void)
{
    // Initialization
#ifndef PLATFORM_WEB
    Settings settings = Settings::LoadFromFile();

    vec2 screenSize = vec2(settings.windowWidth, settings.windowHeight);
    float margin = settings.margin;

    float volume = settings.volume;

    Color mainColour = settings.mainColour;
    Color backgroundColour = settings.backgroundColour;
    Color accentColour = settings.accentColour;
    float lineThickness = settings.lineThickness;

    float dotSize = settings.dotSize;
    float dotSpacing = settings.dotSpacing;

#else
    vec2 screenSize = vec2(300, 500);
    float margin = 30;

    float volume = 0.5;

    Color mainColour = GetColor(0xc0fdfbff);
    Color accentColour = GetColor(0x000000dd);
    Color backgroundColour = GetColor(0x3e6990cc);
    float lineThickness = 6;

    float dotSize = 6;
    float dotSpacing = 16;

#endif

#ifndef PLATFORM_WEB
    SetConfigFlags(FLAG_WINDOW_TRANSPARENT);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetConfigFlags(FLAG_WINDOW_UNDECORATED);
#endif
    InitWindow(screenSize.x, screenSize.y, "Wave Pool");
    SetTargetFPS(60);
    InitAudioDevice();
    SetMasterVolume(volume);


    rect backgroundRect = rect(margin, screenSize - (2 * margin));

#ifndef PLATFORM_WEB
    WindowManager windowManager = WindowManager(vec2(500));
    windowManager.LoadResources();
    windowManager.SetStyle(accentColour, mainColour, lineThickness);
    windowManager.OnWindowResized(screenSize, margin);

    SettingsMenu settingsMenu = SettingsMenu(&settings);
    settingsMenu.LoadResources();
    settingsMenu.SetStyle(accentColour, mainColour, lineThickness);
    settingsMenu.OnWindowResized(screenSize, margin);
#endif

    WavePool wavePool = WavePool(screenSize, margin, dotSize, dotSpacing, mainColour);

    RippleSpawner rippleSpawner = RippleSpawner(&wavePool, 30);

    RadialInstrument radialInstrument= RadialInstrument(&rippleSpawner, screenSize, margin);
    radialInstrument.LoadSounds();


    // Main Game Loop
    while(!WindowShouldClose())
    {
    #ifndef PLATFORM_WEB
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
    #else
        radialInstrument.Update();
        wavePool.Update();
    #endif

        BeginDrawing();

    #ifndef PLATFORM_WEB
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
        if (settings.showFps) {
            auto displayString = utils::GetString(1 / GetFrameTime()).c_str();
            DrawText(displayString, lineThickness, lineThickness, margin - 2 * lineThickness, accentColour);
        }
    #else
        ClearBackground(backgroundColour);
        radialInstrument.DrawGuides(accentColour, lineThickness);
        wavePool.Draw();
    #endif

        EndDrawing();
    }
    // De-Initialization
#ifndef PLATFORM_WEB
    windowManager.UnloadResources();
    settingsMenu.UnloadResources();
#endif
    radialInstrument.UnloadSounds();
    CloseAudioDevice();
    CloseWindow();
    return 0;
}