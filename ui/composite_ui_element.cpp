#include "ui_element.cpp"

namespace ui {
#ifndef COMPOSITE_UI_ELEMENT
#define COMPOSITE_UI_ELEMENT

  class CompositeUIElement: public UIElement
  {
    public:
      virtual void SetStyle(Color, Color, float) = 0;
      virtual void BindCallbacks() = 0;
      virtual void LoadResources() {};
      virtual void UnloadResources() {};
  };
#endif
}