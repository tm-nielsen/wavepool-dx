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
      RippleSpawner(WavePool*, RippleParameters, RippleParameters, RippleParameters, float);
      void SpawnCentreRipple(vec2);
      void SpawnRipple(vec2, float);
      void SpawnHeldRipple(vec2);
  };

  RippleSpawner::RippleSpawner(WavePool* wavePool, RippleParameters clickRippleParameters,
      RippleParameters centreRippleParameters,
      RippleParameters heldRippleParameters, float heldRippleSpawnRate)
  {
    this->wavePool = wavePool;
    this->clickRippleParameters = clickRippleParameters;
    this->centreRippleParameters = centreRippleParameters;
    this->heldRippleParameters = heldRippleParameters;
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