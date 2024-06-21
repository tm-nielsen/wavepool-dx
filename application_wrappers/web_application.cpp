#include "wave_pool_application.cpp"
#include <emscripten.h>

namespace application_wrappers {
  EM_JS(int, GetDocumentWidth, (), {
    return document.body.clientWidth;
  });

  EM_JS(int, GetDocumentHeight, (), {
    return document.body.clientHeight;
  });

  class WebApplication: public WavePoolApplication
  {
    protected:
      vec2 screenSize;
      const float margin = 30;

    public:
      void InitializeApplication()
      {
        screenSize = GetAvailableScreenSpace();
        WavePoolApplication::InitializeApplication(screenSize, margin);
      }

      void Update()
      {
        vec2 availableScreenSpace = GetAvailableScreenSpace();
        if (availableScreenSpace != screenSize) {
          screenSize = availableScreenSpace;
          SetWindowSize(screenSize.x, screenSize.y);
          wavePool->Resize(screenSize, margin);
          radialInstrument->Resize(screenSize, margin);
        }
        WavePoolApplication::Update();
      }

      vec2 GetAvailableScreenSpace()
      {
        int width = GetDocumentWidth();
        int height = GetDocumentHeight();
        return vec2(width, height);
      }
  };
}