#include <functional>
#include "raylib.h"
#include "../utils/vec2.cpp"


#ifndef UI_CALLBACK_TYPES
#define UI_CALLBACK_TYPES

using namespace std::placeholders;
using namespace utils;

typedef std::function<void()> BoundCallback;
typedef std::function<void(float)> BoundFloatCallback;
typedef std::function<void(vec2)> BoundVec2Callback;
typedef std::function<void(const char*)> BoundStringCallback;
typedef std::function<void(Color)> BoundColourCallback;

#endif