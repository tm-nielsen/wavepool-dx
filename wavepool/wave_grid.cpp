#include "raylib.h"
#include "../utils/vec2.cpp"
#include <functional>

namespace wavepool {
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
      void DrawGrid();
      void DrawWarpedGrid(std::function<vec2(vec2)>);
      void UpdateGridSize(vec2);
  };
  
  WaveGrid::WaveGrid(): origin{vec2()}, colour{WHITE}, dotSize{5}, separation{10}, columns{0}, rows{0} {}
  WaveGrid::WaveGrid(vec2 origin, vec2 size, float separation, float dotSize, Color colour):
    origin{origin}, colour{colour}, dotSize{dotSize}, separation{separation}
  {
    UpdateGridSize(size);
  }

  void WaveGrid::UpdateGridSize(vec2 newSize)
  {
    columns = newSize.x / separation;
    rows = newSize.y / separation;

    spacing = newSize / vec2(columns, rows);
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