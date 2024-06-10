#include "raylib.h"
#include <iostream>
#include "vec2.cpp"
#include "wave_pool.cpp"

namespace wavepool {
  class RippleSpawner {
    private:
      RippleParameters clickRippleParameters;
      RippleParameters heldRippleParameters;
      RippleParameters centreRippleParameters;

      WavePool* wavePool;
      vec2 centre_position;

      float heldRippleSpawnDelay;
      double lastHeldRippleSpawnTime;

    public:
      RippleSpawner(WavePool*, RippleParameters, RippleParameters, RippleParameters, vec2, float);
      void SpawnCentreRipple();
      void SpawnRipple(vec2);
      void SpawnHeldRipple(vec2);
  };

  RippleSpawner::RippleSpawner(WavePool* wavePool, RippleParameters clickRippleParameters,
      RippleParameters centreRippleParameters, RippleParameters heldRippleParameters,
      vec2 screenSize, float heldRippleSpawnRate)
  {
    this->wavePool = wavePool;
    this->clickRippleParameters = clickRippleParameters;
    this->centreRippleParameters = centreRippleParameters;
    this->heldRippleParameters = heldRippleParameters;

    centre_position = screenSize / 2;
    heldRippleSpawnDelay = 1.0 / heldRippleSpawnRate;
  }

  void RippleSpawner::SpawnCentreRipple()
  {
    wavePool->AddRipple(Ripple(centre_position, centreRippleParameters));
  }

  void RippleSpawner::SpawnRipple(vec2 spawnPosition)
  {
    wavePool->AddRipple(Ripple(spawnPosition, clickRippleParameters));
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