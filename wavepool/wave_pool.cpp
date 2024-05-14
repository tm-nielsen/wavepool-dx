#include "raylib.h"
#include <vector>
#include <functional>
#include "vec2.cpp"
#include "wave_grid.cpp"
#include "ripple.cpp"

namespace wavepool {
  class WavePool
  {
    private:
      WaveGrid waveGrid;
      std::vector<Ripple> ripples;

      const std::function<vec2(vec2)> getOffset = [this](vec2 point) -> vec2 {return GetOffset(point);};
      vec2 GetOffset(vec2);

    public:
      WavePool(vec2, int, int, int, float, Color);
      ~WavePool();
      void Update();
      void RemoveDeadRipples();
      void Draw();
      void AddRipple(Ripple);
  };

  WavePool::WavePool(vec2 screenSize, int margin, int columns, int rows, float dotRadius, Color dotColour)
  {
    vec2 gridOrigin = vec2(margin) + dotRadius;
    vec2 gridSize = screenSize - margin * 2;
    waveGrid = WaveGrid(gridOrigin, gridSize, columns, rows, dotRadius, dotColour);
  }

  WavePool::~WavePool()
  {
    delete &waveGrid;
    ripples.clear();
  }

  void WavePool::Update()
  {
    RemoveDeadRipples();

    float timeDelta = GetFrameTime();
    for (Ripple& ripple: ripples)
      ripple.Update(timeDelta);
  }

  void WavePool::RemoveDeadRipples()
  {
    for (int i = 0; i < ripples.size(); )
    {
      if (ripples[i].isAlive)
        i++;
      else
      {
        ripples[i] = ripples.back();
        ripples.pop_back();
      }
    }
  }

  void WavePool::Draw()
  {
    waveGrid.DrawWarpedGrid(getOffset);
  }

  void WavePool::AddRipple(Ripple ripple)
  {
    ripples.push_back(ripple);
  }

  vec2 WavePool::GetOffset(vec2 position)
  {
    vec2 offset = vec2();

    for (Ripple& ripple: ripples)
      offset += ripple.GetOffset(position);

    return offset;
  }
}