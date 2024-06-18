#include "button.cpp"

namespace ui {
#ifndef TOGGLE_BUTTON
#define TOGGLE_BUTTON

  class ToggleButton: public Button
  {
    public:
      bool isToggled = false;

      void Press();
      void DrawWithColour(Color);
  };

  void ToggleButton::Press()
  {
    isToggled = !isToggled;
    Button::Press();
  }

  void ToggleButton::DrawWithColour(Color colour)
  {
    area.DrawRounded(borderThickness, colour);
    if (isToggled)
      DrawTexture(colour);
  }
#endif
}