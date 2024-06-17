#include "raylib.h"
#include "../utils/rect.cpp"

namespace ui {
#ifndef LABEL
#define LABEL
  using namespace utils;

  class Label
  {
    private:
      const char* text;
      Color colour;
      rect area;
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
      void Draw(vec2);
  };

  Label::Label(): text{"Label"} {};
  Label::Label(const char* text): text{text} {};


  void Label::SetStyle(Color colour, float margin)
  {
    this->colour = colour;
    this->margin = margin;
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

  void Label::Draw(vec2 offset = vec2())
  {
    vec2 textPosition = area.origin + margin;
    textPosition += offset;
    DrawText(text, textPosition.x, textPosition.y, fontSize, colour);
  }
#endif
}