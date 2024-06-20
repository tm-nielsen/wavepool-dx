#include <math.h>
#include <iostream>
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

      float GetSmallestComponent() {
        if (x < y)
          return x;
        return y;
      }

      vec2 RotatedAroundPoint(vec2 anchorPoint, float rotationDegrees) {
        vec2 offset = *this - anchorPoint;
        return anchorPoint + offset.Rotated(rotationDegrees);
      }

      vec2 Rotated(float rotationDegrees) {
        float radians = rotationDegrees * PI / 180;
        return vec2(x * cos(radians) - y * sin(radians), x * sin(radians) + y * cos(radians));
      }

      vec2 ScaledAroundPoint(vec2 anchorPoint, float scale) {
        vec2 offset = *this - anchorPoint;
        return anchorPoint + offset * scale;
      }

      vec2 ClampLower(vec2 lowerLimit)
      {
        vec2 v = *this;
        if (v.x < lowerLimit.x)
          v.x = lowerLimit.x;
        if (v.y < lowerLimit.y)
          v.y = lowerLimit.y;
        return v;
      }

      vec2 ClampUpper(vec2 upperLimit)
      {
        vec2 v = *this;
        if (v.x > upperLimit.x)
          v.x = upperLimit.x;
        if (v.y > upperLimit.y)
          v.y = upperLimit.y;
        return v;
      }

      Vector2 ToVector2() {
        return Vector2{x, y};
      }
  };

  std::ostream& operator<<(std::ostream &os, const vec2 & v) {
    return os << "(" << v.x << ", " << v.y << ")";
  }
#endif
};