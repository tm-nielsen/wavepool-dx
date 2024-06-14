#include <functional>
#include "raylib.h"
#include "../utils/rect.cpp"

namespace ui {
#ifndef BUTTON
#define BUTTON
  using namespace utils;

  class Button
  {
    private:
      rect area;
      Texture texture;
      float textureScale;
      float hoverRotation;
      float textureRotation;
      bool canPress;

    protected:
      Color normalColour;
      Color hoveredColour;
      float borderThickness;

    public:
      std::function<void()> onPress;
      std::function<void()> onRelease;
      bool isHovered;
      bool isPressed;

      Button();
      Button(rect, std::function<void()>, std::function<void()>);
      Button(vec2, vec2, std::function<void()>, std::function<void()>);
      
      void LoadResources(const char*);
      void UnloadResources();
      void SetStyle(Color, Color, float, float);
      virtual void Update(vec2);
      virtual void OnPressed();
      virtual void OnReleased();
      virtual void OnReleasedWithoutHover();
      void LerpRotation();
      virtual void Draw();
      void DrawWithColour(Color);
  };

  Button::Button(): area{rect()}, onPress{[](){}}, onRelease{[](){}} {};

  Button::Button(rect area,
      std::function<void()> onPress = [](){},
      std::function<void()> onRelease = [](){}):
    area{area}, onPress{onPress}, onRelease{onRelease} {};

  Button::Button(vec2 origin, vec2 size,
      std::function<void()> onPress = [](){},
      std::function<void()> onRelease = [](){}):
    area{rect(origin, size)}, onPress{onPress}, onRelease{onRelease} {};


  void Button::LoadResources(const char* texturePath)
  {
    if (FileExists(texturePath))
      texture = LoadTexture(texturePath);
    else
      texture = LoadTextureFromImage(GenImageColor(12, 12, BLANK));
    textureScale = area.size.y / texture.height;
  }

  void Button::UnloadResources()
  {
    UnloadTexture(texture);
  }

  void Button::SetStyle(Color normal, Color hovered, float thickness, float rotation = -90)
  {
    normalColour = normal;
    hoveredColour = hovered;
    borderThickness = thickness;
    hoverRotation = rotation;
  }

  void Button::Update(vec2 mousePosition)
  {
    isHovered = canPress && area.ContainsPoint(mousePosition);

    bool isMousePressed = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
    if (isHovered) {
      if (isPressed && IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        OnReleased();
      else if (canPress && !isPressed && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        OnPressed();
    }
    else {
      if (!isMousePressed)
      {
        canPress = true;
        if (isPressed)
          OnReleasedWithoutHover();
      }
      else if (!isPressed)
        canPress = false;
    }

    LerpRotation();
  }

  void Button::OnPressed()
  {
    isPressed = true;
    onPress();
  }

  void Button::OnReleased()
  {
    isPressed = false;
    onRelease();
  }

  void Button::OnReleasedWithoutHover()
  {
    isPressed = false;
  }

  void Button::LerpRotation()
  {
    float targetRotation = isHovered? hoverRotation: 0;
    textureRotation = Lerp(textureRotation, targetRotation, 12 * GetFrameTime());
  }

  void Button::Draw()
  {
    Color colour = isHovered? hoveredColour: normalColour;
    DrawWithColour(colour);
  }

  void Button::DrawWithColour(Color colour)
  {
    area.DrawRounded(borderThickness, colour);

    float finalTextureScale = textureScale;
    vec2 centre = area.origin + area.size / 2;
    vec2 texturePosition = area.origin.RotatedAroundPoint(centre, textureRotation);
    if (isPressed) {
      finalTextureScale *= 0.6;
      texturePosition = texturePosition.ScaledAroundPoint(centre, 0.6);
    }
    DrawTextureEx(texture, texturePosition.ToVector2(), textureRotation, finalTextureScale, colour);
  }
#endif
}