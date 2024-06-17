#include "raylib.h"
#include "hoverable_ui_element.cpp"

namespace ui {
#ifndef BUTTON
#define BUTTON
  class Button: public HoverableUIElement
  {
    private:
      Texture texture;
      float textureScale;
      float hoverRotation;
      float textureRotation;
      bool canPress;

    public:
      BoundCallback onPress;
      BoundCallback onRelease;
      bool isPressed = false;

      Button(rect);
      Button(vec2, vec2);
      
      void LoadResources(const char*);
      void UnloadResources();
      void SetStyle(Color, Color, float, float);
      void SetArea(rect);
      virtual void Update(vec2);
      virtual void Press();
      virtual void Release();
      virtual void ReleaseWithoutHover();
      void LerpRotation();
      virtual void Draw();
      void DrawWithColour(Color);
  };

  Button::Button(rect buttonArea = rect()) { SetArea(buttonArea); }
  Button::Button(vec2 origin, vec2 size) { SetArea(rect(origin, size)); }


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
    HoverableUIElement::SetStyle(normal, hovered, thickness);
    hoverRotation = rotation;
  }

  void Button::SetArea(rect area)
  {
    UIElement::SetArea(area);
    textureScale = area.size.y / texture.height;
  }


  void Button::Update(vec2 mousePosition)
  {
    isHovered = canPress && area.ContainsPoint(mousePosition);

    bool isMousePressed = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
    if (isHovered) {
      if (isPressed && IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        Release();
      else if (canPress && !isPressed && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        Press();
    }
    else {
      if (!isMousePressed)
      {
        canPress = true;
        if (isPressed)
          ReleaseWithoutHover();
      }
      else if (!isPressed)
        canPress = false;
    }

    LerpRotation();
  }

  void Button::Press()
  {
    isPressed = true;
    if (onPress) onPress();
  }

  void Button::Release()
  {
    isPressed = false;
    if (onRelease) onRelease();
  }

  void Button::ReleaseWithoutHover()
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