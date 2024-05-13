#include "raylib.h"
#include "raymath.h"
#include "wavepool/wave_grid.cpp"

using namespace wavepool;

int main(void)
{
    // Initialization
    Vector2 screenSize = Vector2{800, 450};
    InitWindow(screenSize.x, screenSize.y, "raylib cpp test");
    SetTargetFPS(60);

    float margin = 20;
    Vector2 gridOrigin = Vector2{margin, margin};
    Vector2 gridSize = Vector2{screenSize.x - 2 * margin, screenSize.y - 2 * margin};
    WaveGrid waveGrid = WaveGrid(gridOrigin, gridSize, 64, 36, 3, LIME);

    // Main Game Loop
    while(!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(DARKGRAY);
        waveGrid.DrawGrid();

        EndDrawing();
    }
    // De-Initialization
    CloseWindow();
    return 0;
}