#include <math.h>
#include "raymath.h"

namespace utils {
#ifndef VEC_2
#define VEC_2

#define UP vec2(0, -1)
#define DOWN vec2(0, 1)
#define RIGHT vec2(1, 0)
#define LEFT vec2(-1, 0)

  struct vec2
  {
    public:
      float x;
      float y;

      vec2(): x{0}, y{0} {};
      vec2(float s): x{s}, y{s} {};
      vec2(int s): x{float(s)}, y{float(s)} {};
      vec2(float x, float y): x{x}, y{y} {};
      vec2(int x, int y): x{float(x)}, y{float(y)} {};
      vec2(const vec2& v): x{v.x}, y{v.y} {};

      vec2(const Vector2& v): x{v.x}, y{v.y} {};
      vec2 operator=(Vector2 v) {
        x = v.x;
        y = v.y;
        return *this;
      }

      vec2 operator=(vec2 v) {
        x = v.x;
        y = v.y;
        return *this;
      }

      vec2 operator+(vec2 v) {
        return vec2(x + v.x, y + v.y);
      }
      vec2 operator-(vec2 v) {
        return vec2(x - v.x, y - v.y);
      }
      vec2 operator*(vec2 v) {
        return vec2(x * v.x, y * v.y);
      }
      vec2 operator/(vec2 v) {
        return vec2(x / v.x, y / v.y);
      }

      vec2 operator+=(vec2 v) {
        x += v.x;
        y += v.y;
        return *this;
      }
      vec2 operator-=(vec2 v) {
        x -= v.x;
        y -= v.y;
        return *this;
      }
      vec2 operator*=(vec2 v) {
        x *= v.x;
        y *= v.y;
        return *this;
      }
      vec2 operator/=(vec2 v) {
        x /= v.x;
        y /= v.y;
        return *this;
      }

      vec2 operator+(float s) {
        return vec2(x + s, y + s);
      }
      vec2 operator-(float s) {
        return vec2(x - s, y - s);
      }
      vec2 operator*(float s) {
        return vec2(x * s, y * s);
      }
      vec2 operator/(float s) {
        return vec2(x / s, y / s);
      }

      vec2 operator+=(float s) {
        x += s;
        y += s;
        return *this;
      }
      vec2 operator-=(float s) {
        x -= s;
        y -= s;
        return *this;
      }
      vec2 operator*=(float s) {
        x *= s;
        y *= s;
        return *this;
      }
      vec2 operator/=(float s) {
        x /= s;
        y /= s;
        return *this;
      }

      bool operator==(vec2 v) {
        return x == v.x && y == v.y;
      }
      bool operator!=(vec2 v) {
        return x != v.x || y != v.y;
      }

      vec2 Normalized() {
        float length = Length();
        if (length == 0) return *this;
        return *this / length;
      }

      float DistanceTo(vec2 v) {
        return vec2(v.x - x, v.y - y).Length();
      }
      float DistanceToSquared(vec2 v) {
        float xDist = v.x - x;
        float yDist = v.y - y;
        return xDist * xDist + yDist * yDist;
      }

      float Length() {
        return std::sqrt(x * x + y * y);
      }
      float LengthSquared() {
        return x * x + y * y;
      }

      float Dot(vec2 v) {
        return x * v.x + y * v.y;
      }

      float GetLargestComponent() {
        if (x > y)
          return x;
        return y;
      }

      Vector2 ToVector2() {
        return Vector2{x, y};
      }
  };
#endif
};