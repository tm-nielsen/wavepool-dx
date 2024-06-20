#include "wave_pool_application.cpp"

namespace application_wrappers {
  class WebApplication: public WavePoolApplication
  {
    public:
      void InitializeApplication()
      {
        WavePoolApplication::InitializeApplication(vec2(300, 500), 30, 0.5);
      }
  };
}