#include "raylib.h"
#include "../wave_pool/radial_instrument.cpp"
#include <iostream>

using namespace wave_pool;
using namespace utils;

namespace application_wrappers {
  class WavePoolApplication {
    protected:
      WavePool* wavePool = nullptr;
      RippleSpawner* rippleSpawner = nullptr;
      RadialInstrument* radialInstrument = nullptr;

      Color backgroundColour;

    public:
      bool shouldExit = false;

      WavePoolApplication() {};
      ~WavePoolApplication()
      {
        delete wavePool;
        delete rippleSpawner;
        delete radialInstrument;
      }

      void InitializeApplication(
        vec2 screenSize = vec2(600),
        float margin = 40,
        float volume = 0.5,
        Color mainColour = GetColor(0xc0fdfbff),
        Color backgroundColour = GetColor(0x3e6990cc),
        Color accentColour = GetColor(0x000000dd),
        float lineThickness = 6,
        float dotSize = 6, float dotSpacing = 16)
      {
        std::cout << "\n\nInitializing application\n\n\n";
        this->backgroundColour = backgroundColour;

        InitWindow(screenSize.x, screenSize.y, "Wave Pool");
        SetTargetFPS(60);
        InitAudioDevice();
        SetMasterVolume(volume);
        
        wavePool = new WavePool(screenSize, margin, dotSize, dotSpacing, mainColour);

        rippleSpawner = new RippleSpawner(wavePool, 30);

        radialInstrument = new RadialInstrument(rippleSpawner, screenSize, margin);
        radialInstrument->SetStyle(accentColour, lineThickness);
        radialInstrument->LoadSounds();
      }

      void EndApplication()
      {
        UnloadResources();
        CloseAudioDevice();
        CloseWindow();
      }

      virtual void UnloadResources()
      {
        radialInstrument->UnloadSounds();
      }

      virtual void Update()
      {
        radialInstrument->Update();
        wavePool->Update();
      }

      virtual void Draw()
      {
        BeginDrawing();

        ClearBackground(backgroundColour);
        radialInstrument->Draw();
        wavePool->Draw();

        EndDrawing();
      }
  };
}