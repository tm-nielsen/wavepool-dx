#include "wave_pool_application.cpp"

namespace application_wrappers {
  class WebApplication: public WavePoolApplication
  {
    public:
      // WebApplication()
      // {
      //   InitializeApplication(vec2(300, 500), 30, 0.5);
      // }

      void InitializeApplication()
      {
        WavePoolApplication::InitializeApplication(vec2(300, 500), 30, 0.5);
      }
  };
}