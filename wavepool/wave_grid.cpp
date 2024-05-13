#include "raylib.h"
#include "raymath.h"
// #include <vector>

namespace wavepool {
  class WaveGrid
  {
    private:
      Vector2 origin;
      Vector2 spacing;

      Color colour;
      float dotSize;

      int rows;
      int columns;

    public:
      WaveGrid(Vector2, Vector2, int, int, float, Color);
      ~WaveGrid();

      void DrawGrid();
  };
  
  WaveGrid::WaveGrid(Vector2 origin, Vector2 size, int columns, int rows, float dotSize, Color colour):
    origin{origin}, rows{rows}, columns{columns}, colour{colour}, dotSize{dotSize}
  {
    spacing = Vector2{size.x / columns, size.y / rows};
  }
  
  WaveGrid::~WaveGrid()
  {
  }
  
  void WaveGrid::DrawGrid()
  {
    for (int i = 0; i < columns; i++) {
      for (int j = 0; j < rows; j++) {
        float x = i * spacing.x + origin.x;
        float y = j * spacing.y + origin.y;
        DrawCircle(x, y, dotSize, colour);
      }
    }
  }
}