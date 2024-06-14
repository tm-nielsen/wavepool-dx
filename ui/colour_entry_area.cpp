#include "text_area.cpp"

namespace ui {
  class ColourEntryArea: public TextArea
  {
    void PasteFromClipboard() override;
    void CopyToClipboard() override;
    bool IsCharacterValid(char) override;
    vec2 DrawEnteredText(Color) override;
  };

  void ColourEntryArea::PasteFromClipboard()
  {
    TextArea::PasteFromClipboard();
    if (text[0] == '#')
      text = text.substr(1, text.length() -1);
  }

  void ColourEntryArea::CopyToClipboard()
  {
    SetClipboardText(("#" + text).c_str());
  }

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