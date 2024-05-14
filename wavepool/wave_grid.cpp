#include "raylib.h"
#include "vec2.cpp"
#include <functional>

namespace wavepool {
  class WaveGrid
  {
    private:
      vec2 origin;
      vec2 spacing;

      Color colour;
      float dotSize;

      int columns;
      int rows;

    public:
      WaveGrid();
      WaveGrid(vec2, vec2, int, int, float, Color);
      void DrawGrid();
      void DrawWarpedGrid(std::function<vec2( vec2)>);
  };
  
  WaveGrid::WaveGrid(): origin{vec2()}, colour{WHITE}, dotSize{5}, columns{0}, rows{0} {}
  WaveGrid::WaveGrid(vec2 origin, vec2 size, int columns, int rows, float dotSize, Color colour):
    origin{origin}, colour{colour}, dotSize{dotSize}, columns{columns}, rows{rows}
  {
    spacing = size / vec2(columns, rows);
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