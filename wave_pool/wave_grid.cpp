#include "raylib.h"
#include "../utils/vec2.cpp"
#include <functional>

namespace wave_pool {
  using namespace utils;

  class WaveGrid
  {
    private:
      vec2 origin;
      vec2 unusedSpace;

      Color colour;
      float dotSize;
      float spacing;

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
  
  WaveGrid::WaveGrid(): origin{vec2()}, colour{WHITE}, dotSize{5}, spacing{10}, columns{0}, rows{0} {}
  WaveGrid::WaveGrid(vec2 origin, vec2 size, float spacing, float dotSize, Color colour):
    origin{origin}, colour{colour}, dotSize{dotSize}, spacing{spacing}
  {
    UpdateGridSize(size);
  }

  void WaveGrid::UpdateGridSize(vec2 newSize)
  {
    newSize -= dotSize * 2;
    columns = 1 + newSize.x / spacing;
    rows = 1 + newSize.y / spacing;
    vec2 grid = vec2(columns, rows);

    unusedSpace = newSize - (grid - 1) * spacing;
  }

  void WaveGrid::SetLayoutValues(float size, float separation)
  {
    vec2 currentSize = (vec2(columns, rows) - 1) * spacing;
    currentSize += unusedSpace;
    currentSize += dotSize * 2;
    dotSize = size;
    spacing = separation;
    UpdateGridSize(currentSize);
  }

  void WaveGrid::SetColour(Color newColour)
  {
    colour = newColour;
  }
  
  void WaveGrid::DrawGrid()
  {
    vec2 basePosition = origin + dotSize + unusedSpace / 2;
    for (int i = 0; i < columns; i++) {
      for (int j = 0; j < rows; j++) {
        vec2 point = vec2(i, j) * spacing;
        point += basePosition;
        DrawCircle(point.x, point.y, dotSize, colour);
      }
    }
  }
  void WaveGrid::DrawWarpedGrid(std::function<vec2(vec2)> getOffset)
  {
    vec2 basePosition = origin + dotSize + unusedSpace / 2;
    for (int i = 0; i < columns; i++) {
      for (int j = 0; j < rows; j++) {
        vec2 point = vec2(i, j) * spacing;
        point += basePosition;
        point += getOffset(point);
        DrawCircle(point.x, point.y, dotSize, colour);
      }
    }
  }
}