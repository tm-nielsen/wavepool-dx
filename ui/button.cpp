#include <functional>
#include "raylib.h"
#include "../utils/rect.cpp"

namespace ui {
  using namespace utils;

  class Button
  {
    private:
      rect area;
      Texture texture;
      float textureScale;
      float hoverRotation;
      float textureRotation;

      Color normalColour;
      Color hoveredColour;
      float borderThickness;

    public:
      std::function<void()> onClick;
      bool isHovered;
      bool isPressed;

      Button();
      Button(vec2, vec2, std::function<void()>);
      void LoadTextureFromFile(const char*);
      void SetStyle(Color, Color, float, float);
      void Update(vec2);
      void Draw();
  };

  Button::Button(): area{rect()}, onClick{[]() {}} {}

  Button::Button(vec2 origin, vec2 size, std::function<void()> onClick):
    area{rect(origin, size)}, onClick{onClick} {}


  void Button::LoadTextureFromFile(const char* texturePath) {
    if (FileExists(texturePath))
      texture = LoadTexture(texturePath);
    else
      texture = LoadTextureFromImage(GenImageColor(16, 16, BLANK));
    textureScale = area.size.y / texture.height;
  }

  void Button::SetStyle(Color normal, Color hovered, float thickness, float rotation = 30) {
    normalColour = normal;
    hoveredColour = hovered;
    borderThickness = thickness;
    hoverRotation = rotation;
  }

  void Button::Update(vec2 mousePosition) {
    isHovered = area.ContainsPoint(mousePosition);
    if (isPressed && !IsMouseButtonDown(MOUSE_BUTTON_LEFT))
      onClick();
    isPressed = IsMouseButtonDown(MOUSE_BUTTON_LEFT) && isHovered;

    float targetRotation = isHovered? hoverRotation: 0;
    textureRotation = Lerp(textureRotation, targetRotation, 0.01);
  }

  void Button::Draw() {
    Color colour = isHovered? hoveredColour : normalColour;
    area.DrawRounded(borderThickness, colour);
    DrawTextureEx(texture, area.origin.ToVector2(), textureRotation, textureScale, colour);
  }
}