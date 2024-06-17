#include "raylib.h"
#include "ui_element.cpp"

namespace ui {
#ifndef LABEL
#define LABEL
  using namespace utils;

  class Label: public UIElement
  {
    private:
      const char* text;
      float margin = 12;
      float fontSize = 20;

    public:
      Label();
      Label(const char*);

      void SetStyle(Color, float);
      void SetArea(rect);
      void SetText(const char*);
      float GetFontSize();
      float GetTextLength();
      vec2 GetTextEnd();
      void Draw();
  };

  Label::Label(): text{"Label"} {};
  Label::Label(const char* text): text{text} {};


  void Label::SetStyle(Color colour, float fontMargin = 12)
  {
    normalColour = colour;
    margin = fontMargin;
    fontSize = GetFontSize();
  }

  void Label::SetArea(rect area)
  {
    this->area = area;
    fontSize = GetFontSize();
  }

  void Label::SetText(const char* newText)
  {
    text = newText;
  }

  float Label::GetFontSize()
  {
    return area.size.y - 2 * margin;
  }

  float Label::GetTextLength()
  {
    return MeasureText(text, fontSize);
  }

  vec2 Label::GetTextEnd()
  {
    vec2 textOrigin = area.origin + margin;
    return textOrigin + RIGHT * GetTextLength();
  }

  void Label::Draw()
  {
    vec2 textPosition = area.origin + margin;
    DrawText(text, textPosition.x, textPosition.y, fontSize, normalColour);
  }
#endif
}