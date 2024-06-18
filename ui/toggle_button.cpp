#include "button.cpp"

namespace ui {
#ifndef TOGGLE_BUTTON
#define TOGGLE_BUTTON

  class ToggleButton: public Button
  {
    public:
      BoundBooleanCallback onToggle;
      bool isToggled = false;

      void Press();
      void DrawWithColour(Color);
  };

  void ToggleButton::Press()
  {
    isToggled = !isToggled;
    if (onToggle) onToggle(isToggled);
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