#include "raylib.h"
#include <vector>
#include <functional>
#include "../utils/vec2.cpp"
#include "wave_grid.cpp"
#include "ripple.cpp"

namespace wave_pool {
  using namespace utils;
  using namespace std::placeholders;

  class WavePool
  {
    private:
      WaveGrid waveGrid;
      std::vector<Ripple> ripples;

      vec2 GetOffset(vec2);

    public:
      WavePool(vec2, float, float, float, Color);
      void Update();
      void RemoveDeadRipples();
      void OnWindowResized(vec2, float);
      void SetColour(Color);
      void SetGridLayout(float, float);
      void Draw();
      void AddRipple(Ripple);
  };

  WavePool::WavePool(vec2 screenSize, float margin, float dotRadius, float dotSpacing, Color dotColour)
  {
    ripples = std::vector<Ripple>();
    waveGrid = WaveGrid(vec2(margin), vec2(), dotSpacing, dotRadius, dotColour);
    OnWindowResized(screenSize, margin);
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

  void WavePool::OnWindowResized(vec2 screenSize, float margin)
  {
    waveGrid.SetOrigin(vec2(margin));
    waveGrid.UpdateGridSize(screenSize - margin * 2);
  }

  void WavePool::SetColour(Color colour)
  {
    waveGrid.SetColour(colour);
  }

  void WavePool::SetGridLayout(float size, float separation)
  {
    waveGrid.SetLayoutValues(size, separation);
  }

  void WavePool::Draw()
  {
    auto getOffset = std::bind(&WavePool::GetOffset, this, std::placeholders::_1);
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