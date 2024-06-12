#include "raylib.h"
#include "vec2.cpp"

namespace wavepool {
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

      void Draw(float thickness, Color colour) {
        DrawRectangleLinesEx(ToRectangle(), thickness, colour);
      }

      Rectangle ToRectangle() {
        return Rectangle{origin.x, origin.y, size.x, size.y};
      }
  };
#endif
}