#include "raylib.h"
#include "ui_callback_types.cpp"
#include "../utils/rect.cpp"

namespace ui {
#ifndef BUTTON
#define BUTTON
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
      BoundCallback onPress;
      BoundCallback onRelease;
      bool isHovered = false;
      bool isPressed = false;

      Button();
      Button(rect);
      Button(vec2, vec2);
      
      void LoadResources(const char*);
      void UnloadResources();
      void SetStyle(Color, Color, float, float);
      void SetArea(rect);
      vec2 GetCentrePosition();
      void SetCentrePosition(vec2);
      virtual void Update(vec2);
      virtual void Press();
      virtual void Release();
      virtual void ReleaseWithoutHover();
      void LerpRotation();
      virtual void Draw();
      void DrawWithColour(Color);
  };

  Button::Button(): area{rect()} {};
  Button::Button(rect area): area{area} {};
  Button::Button(vec2 origin, vec2 size): area{rect(origin, size)} {};


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

  void Button::SetArea(rect area)
  {
    this->area = area;
    textureScale = area.size.y / texture.height;
  }
  
  vec2 Button::GetCentrePosition()
  {
    return area.GetCentre();
  }

  void Button::SetCentrePosition(vec2 position)
  {
    area.SetCentre(position);
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