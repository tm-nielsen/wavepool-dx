#include "raylib.h"
#include "../utils/vec2.cpp"
#include <functional>

namespace wave_pool {
  using namespace utils;

  class WaveGrid
  {
    private:
      vec2 origin;
      vec2 spacing;

      Color colour;
      float dotSize;
      float separation;

      int columns;
      int rows;

    public:
      WaveGrid();
      WaveGrid(vec2, vec2, float, float, Color);
      void UpdateGridSize(vec2);
      void SetLayoutValues(float, float);
      void SetColour(Color);
      void DrawGrid();
      void DrawWarpedGrid(std::function<vec2(vec2)>);
  };
  
  WaveGrid::WaveGrid(): origin{vec2()}, colour{WHITE}, dotSize{5}, separation{10}, columns{0}, rows{0} {}
  WaveGrid::WaveGrid(vec2 origin, vec2 size, float separation, float dotSize, Color colour):
    colour{colour}, dotSize{dotSize}, separation{separation}
  {
    this->origin = origin + dotSize;
    UpdateGridSize(size);
  }

  void WaveGrid::UpdateGridSize(vec2 newSize)
  {
    newSize -= dotSize;
    columns = newSize.x / separation;
    rows = newSize.y / separation;

    spacing = newSize / vec2(columns, rows);
  }

  void WaveGrid::SetLayoutValues(float size, float separation)
  {
    vec2 currentSize = spacing * vec2(columns, rows) + dotSize;
    dotSize = size;
    this->separation = separation;
    UpdateGridSize(currentSize);
  }

  void WaveGrid::SetColour(Color newColour)
  {
    colour = newColour;
  }
  
  void WaveGrid::DrawGrid()
  {
    for (int i = 0; i < columns; i++) {
      for (int j = 0; j < rows; j++) {
        vec2 point = vec2(i, j) * spacing;
        point += origin;
        DrawCircle(point.x, point.y, dotSize, colour);
      }
    }
  }
  void WaveGrid::DrawWarpedGrid(std::function<vec2(vec2)> getOffset)
  {
    for (int i = 0; i < columns; i++) {
      for (int j = 0; j < rows; j++) {
        vec2 point = vec2(i, j) * spacing;
        point += origin;
        point += getOffset(point);
        DrawCircle(point.x, point.y, dotSize, colour);
      }
    }
  }
}