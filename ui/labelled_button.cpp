#include "button.cpp"
#include "label.cpp"

namespace ui {
  class LabelledButton: public Button
  {
    private:
      Label label;

    public:
      LabelledButton(rect);
      LabelledButton(const char*, rect);

      void SetStyle(Color, Color, float);
      void SetArea(rect);
      void Draw();
  };

  LabelledButton::LabelledButton(rect buttonArea = rect())
  {
    label = Label();
    SetArea(buttonArea);
  }

  LabelledButton::LabelledButton(const char* labelText, rect buttonArea = rect())
  {
    label = Label(labelText);
    SetArea(buttonArea);
  }

  void LabelledButton::SetStyle(Color normal, Color hovered, float thickness)
  {
    Button::SetStyle(normal, hovered, thickness, 0);
    label.SetStyle(normal);
  }

  void LabelledButton::SetArea(rect area)
  {
    Button::SetArea(area);
    label.SetArea(area);
  }

  void LabelledButton::Draw()
  {
    Color drawColour = isHovered? hoveredColour: normalColour;
    float fontMargin = borderThickness * (isPressed? 3: 2);
    DrawWithColour(drawColour);
    label.SetStyle(drawColour, fontMargin);
    label.Draw();
  }
}