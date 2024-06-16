#include "raylib.h"
#include <iostream>
#include "vec2.cpp"

namespace utils {
#ifndef RECT
#define RECT
  struct rect
  {
    public:
      vec2 origin;
      vec2 size;

      rect(): origin{vec2()}, size{vec2()} {};
      rect(float x, float y, float w, float h): origin{vec2(x, y)}, size{vec2(w, h)} {};
      rect(int x, int y, int w, int h): origin{vec2(x, y)}, size{vec2(w, h)} {};
      rect(vec2 origin, vec2 size): origin{vec2(origin)}, size{vec2(size)} {};
      rect(const rect& r): origin{vec2(r.origin)}, size{vec2(r.size)} {};

      rect(const Rectangle& r): origin{vec2(r.x, r.y)}, size{r.width, r.height}{};
      rect operator=(Rectangle r) {
        origin = vec2(r.x, r.y);
        size = vec2(r.width, r.height);
        return *this;
      }

      rect operator=(rect r) {
        origin = vec2(r.origin);
        size = vec2(r.size);
        return *this;
      }

      bool ContainsPoint(vec2 point) {
        vec2 limit = origin + size;
        return point.x > origin.x && point.y > origin.y
          && point.x < limit.x && point.y < limit.y;
      }

      void GrowRadially(vec2 sizeIncrease) {
        origin -= sizeIncrease;
        size += sizeIncrease * 2;
      }

      vec2 GetCentre() {
        return origin + size / 2;
      }

      void SetCentre(vec2 position) {
        origin = position - size / 2;
      }

      vec2 ClampPoint(vec2 point) {
        point = origin.ClampLower(point);
        point = (origin + size).ClampUpper(point);
        return point;
      }

      void Draw(float thickness, Color colour) {
        DrawRectangleLinesEx(ToRectangle(), thickness, colour);
      }

      void DrawRounded(float thickness, Color colour, float roundness = 0.075, int segments = 4)
      {
        DrawRectangleRoundedLines(ToRectangle(), roundness, segments, thickness, colour);
      }

      void DrawHorizontalBorders(float thickness, Color colour) {
        DrawParallelLines(origin, origin + size * RIGHT, size * DOWN, thickness, colour);
      }

      void DrawVerticalBorders(float thickness, Color colour) {
        DrawParallelLines(origin, origin + size * DOWN, size * RIGHT, thickness, colour);
      }

      void DrawParallelLines(vec2 startPos, vec2 endPos, vec2 offset, float thickness, Color colour) {
        DrawLine(startPos, endPos, thickness, colour);
        startPos += offset;
        endPos += offset;
        DrawLine(startPos, endPos, thickness, colour);
      }

      void DrawLine(vec2 startPos, vec2 endPos, float thickness, Color colour) {
        DrawLineEx(startPos.ToVector2(), endPos.ToVector2(), thickness, colour);
      }

      Rectangle ToRectangle() {
        return Rectangle{origin.x, origin.y, size.x, size.y};
      }
  };
      
  std::ostream& operator<<(std::ostream &os, const rect &r) {
    return os << "[" << r.origin << ", " << r.size << "]";
  }
#endif
}