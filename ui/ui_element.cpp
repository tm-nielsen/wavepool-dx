#include "raylib.h"
#include "../utils/rect.cpp"

namespace ui {
#ifndef UI_ELEMENT
#define UI_ELEMENT
  using namespace utils;

  class UIElement
  {
    protected:
      rect area;
      Color normalColour;
      float borderThickness;

    public:
      UIElement(rect);
      UIElement(vec2, vec2);

      virtual void SetStyle(Color, float);
      virtual void SetArea(rect);
      virtual vec2 GetCentrePosition();
      virtual void SetCentrePosition(vec2);
      virtual void Update(vec2) {};
      virtual void Draw() = 0;
  };

  UIElement::UIElement(rect area = rect()) { SetArea(area); }
  UIElement::UIElement(vec2 origin, vec2 size) { SetArea(rect(origin, size)); }
  

  void UIElement::SetStyle(Color colour, float thickness)
  {
    normalColour = colour;
    borderThickness = thickness;
  }

  void UIElement::SetArea(rect newArea)
  {
    area = newArea;
  }

  vec2 UIElement::GetCentrePosition()
  {
    return area.GetCentre();
  }

  void UIElement::SetCentrePosition(vec2 centre)
  {
    area.SetCentre(centre);
  }
#endif
}