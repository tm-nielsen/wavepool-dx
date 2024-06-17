#include "ui_element.cpp"
#include "ui_callback_types.cpp"

namespace ui {
#ifndef HOVERABLE_UI_ELEMENT
#define HOVERABLE_UI_ELEMENT

  class HoverableUIElement: public UIElement
  {
    protected:
      Color hoveredColour;

    public:
      bool isHovered = false;
      BoundCallback onHoverEnter;
      BoundCallback onHoverExit;

      virtual void SetStyle(Color, Color, float);
      virtual void Update(vec2);
  };

  void HoverableUIElement::SetStyle(Color normal, Color hovered, float thickness)
  {
    UIElement::SetStyle(normal, thickness);
    hoveredColour = hovered;
  }

  void HoverableUIElement::Update(vec2 mousePosition)
  {
    if (area.ContainsPoint(mousePosition)) {
      if (!isHovered) {
        isHovered = true;
        if (onHoverEnter) onHoverEnter();
      }
    }
    else if (isHovered) {
      isHovered = false;
      if (onHoverExit) onHoverExit();
    }
  }
#endif
}