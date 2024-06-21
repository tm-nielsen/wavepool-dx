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
    public:
      void InitializeApplication()
      {
        int width = GetDocumentWidth();
        int height = GetDocumentHeight();
        WavePoolApplication::InitializeApplication(vec2(width, height), 30, 0.5);
      }

      void Update()
      {
        WavePoolApplication::Update();
      }
  };
}