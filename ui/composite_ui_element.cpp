#include "ui_element.cpp"

namespace ui {
#ifndef COMPOSITE_UI_ELEMENT
#define COMPOSITE_UI_ELEMENT

  class CompositeUIElement: public UIElement
  {
    public:
      virtual void BindCallbacks() = 0;
  };
#endif
}