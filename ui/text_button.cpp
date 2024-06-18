#include "button.cpp"
#include "label.cpp"

namespace ui {
  class TextButton: public Button
  {
    private:
      Label label;

    public:
      TextButton(rect);
      TextButton(const char*, rect);

      void SetStyle(Color, Color, float);
      void SetArea(rect);
      void Draw();
  };

  TextButton::TextButton(rect buttonArea = rect())
  {
    label = Label();
    SetArea(buttonArea);
  }

  TextButton::TextButton(const char* labelText, rect buttonArea = rect())
  {
    label = Label(labelText);
    SetArea(buttonArea);
  }

  void TextButton::SetStyle(Color normal, Color hovered, float thickness)
  {
    Button::SetStyle(normal, hovered, thickness, 0);
    label.SetStyle(normal);
  }

  void TextButton::SetArea(rect area)
  {
    Button::SetArea(area);
    label.SetArea(area);
  }

  void TextButton::Draw()
  {
    Color colour = isHovered? hoveredColour: baseColour;
    float fontMargin = borderThickness * (isPressed? 3: 2);
    DrawWithColour(colour);
    label.SetStyle(colour, fontMargin);
    label.Draw();
  }
}