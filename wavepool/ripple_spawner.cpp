#include "raylib.h"
#include "../utils/vec2.cpp"
#include "wave_pool.cpp"

namespace wavepool {
  using namespace utils;

  class RippleSpawner {
    private:
      RippleParameters clickRippleParameters;
      RippleParameters heldRippleParameters;
      RippleParameters centreRippleParameters;

      WavePool* wavePool;

      float heldRippleSpawnDelay;
      double lastHeldRippleSpawnTime;

    public:
      RippleSpawner(WavePool*, float);
      void SpawnCentreRipple(vec2);
      void SpawnRipple(vec2, float);
      void SpawnHeldRipple(vec2);
  };

  RippleSpawner::RippleSpawner(WavePool* wavePool, float heldRippleSpawnRate)
  {
    this->wavePool = wavePool;
    clickRippleParameters = RippleParameters(10, 6, 200, 30, 1, 3);
    centreRippleParameters = RippleParameters(20, 30, 200, 40, 5, 5);
    heldRippleParameters = RippleParameters(10, 3, 200, 20, 1, 1.5);
    heldRippleSpawnDelay = 1.0 / heldRippleSpawnRate;
  }

  void RippleSpawner::SpawnCentreRipple(vec2 centrePosition)
  {
    wavePool->AddRipple(Ripple(centrePosition, centreRippleParameters));
  }

  void RippleSpawner::SpawnRipple(vec2 spawnPosition, float pitchScale = 1)
  {
    wavePool->AddRipple(Ripple(spawnPosition, clickRippleParameters, pitchScale));
  }

  void RippleSpawner::SpawnHeldRipple(vec2 spawnPosition)
  {
    double current_time = GetTime();
    double time_difference = current_time - lastHeldRippleSpawnTime;
    if (time_difference > heldRippleSpawnDelay)
    {
      lastHeldRippleSpawnTime = current_time;
      wavePool->AddRipple(Ripple(spawnPosition, heldRippleParameters));
    }
  }
}