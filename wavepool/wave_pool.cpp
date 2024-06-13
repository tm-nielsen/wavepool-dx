#include "raylib.h"
#include <vector>
#include <functional>
#include "../utils/vec2.cpp"
#include "wave_grid.cpp"
#include "ripple.cpp"

namespace wavepool {
  using namespace utils;

  class WavePool
  {
    private:
      WaveGrid waveGrid;
      std::vector<Ripple> ripples;

      const std::function<vec2(vec2)> getOffset = [this](vec2 point) -> vec2 {return GetOffset(point);};
      vec2 GetOffset(vec2);

      float margin;

    public:
      WavePool(vec2, float, float, float, Color);
      ~WavePool();
      void Update();
      void RemoveDeadRipples();
      void OnWindowResized(vec2);
      void Draw();
      void AddRipple(Ripple);
  };

  WavePool::WavePool(vec2 screenSize, float margin, float dotRadius, float dotSpacing, Color dotColour):
    margin{margin}
  {
    this->margin = margin;
    vec2 gridOrigin = vec2(margin) + dotRadius;
    waveGrid = WaveGrid(gridOrigin, vec2(), dotSpacing, dotRadius, dotColour);
    OnWindowResized(screenSize);
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

  void WavePool::OnWindowResized(vec2 screenSize)
  {
    waveGrid.UpdateGridSize(screenSize - margin * 2);
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