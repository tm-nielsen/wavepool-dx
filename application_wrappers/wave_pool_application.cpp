#include "raylib.h"
#include "../wave_pool/radial_instrument.cpp"

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
        Color mainColour = GetColor(0x6fa79dff),
        Color backgroundColour = GetColor(0x404044ff),
        Color accentColour = GetColor(0x606066ff),
        float lineThickness = 6,
        float dotSize = 6, float dotSpacing = 16)
      {
        this->backgroundColour = backgroundColour;

        InitWindow(screenSize.x, screenSize.y, "Wave Pool");
        SetTargetFPS(60);
        InitAudioDevice();
        SetMasterVolume(volume);
        SetGesturesEnabled(GESTURE_TAP);
        
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
        int touchCount = GetTouchPointCount();
        if (touchCount > 0) {
          if (GetGestureDetected() == GESTURE_TAP) {
            vec2 tapPosition = GetTouchPosition(0);
            radialInstrument->OnClick(tapPosition);
          }
          else {
            for (int i = 0; i < touchCount; i++) {
              vec2 touchPosition = GetTouchPosition(i);
              radialInstrument->OnHold(touchPosition);
            }
          }
        }
        else {
          vec2 mousePosition = GetMousePosition();
          if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            radialInstrument->OnClick(mousePosition);
          }
          else if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            radialInstrument->OnHold(mousePosition);
          }
        }

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