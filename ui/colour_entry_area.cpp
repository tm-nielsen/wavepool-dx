#include "text_area.cpp"

namespace ui {
  class ColourEntryArea: public TextArea
  {
    bool IsCharacterValid(char) override;
    vec2 DrawEnteredText(Color) override;
  };

  bool ColourEntryArea::IsCharacterValid(char c)
  {
    bool isHexLetter = c >= 'a' && c <= 'f';
    isHexLetter |= c >= 'A' && c <= 'F';
    bool isNumber = c >= '0' && c <= '9';
    return isHexLetter || isNumber;
  }

  vec2 ColourEntryArea::DrawEnteredText(Color colour)
  {
    vec2 textPosition = area.origin + margin;
    DrawText("#", textPosition.x, textPosition.y, fontSize, normalColour);
    textPosition += RIGHT * (MeasureText("#", fontSize) + margin);
    DrawText(text.c_str(), textPosition.x, textPosition.y, fontSize, colour);
    return textPosition + RIGHT * MeasureText(text.c_str(), fontSize);
  }
}