#include "text_input_field.cpp"

namespace ui {
  class ColourEntryField: public TextInputField
  {
    void PasteFromClipboard() override;
    void CopyToClipboard() override;
    bool IsCharacterValid(char) override;
    vec2 DrawEnteredText(Color) override;
  };

  void ColourEntryField::PasteFromClipboard()
  {
    TextInputField::PasteFromClipboard();
    if (text[0] == '#')
      text = text.substr(1, text.length() -1);
  }

  void ColourEntryField::CopyToClipboard()
  {
    SetClipboardText(("#" + text).c_str());
  }

  bool ColourEntryField::IsCharacterValid(char c)
  {
    bool isHexLetter = c >= 'a' && c <= 'f';
    isHexLetter |= c >= 'A' && c <= 'F';
    bool isNumber = c >= '0' && c <= '9';
    return isHexLetter || isNumber;
  }

  vec2 ColourEntryField::DrawEnteredText(Color colour)
  {
    vec2 textPosition = area.origin + margin;
    Color prefixColour = isFocused? normalColour: colour;
    DrawText("#", textPosition.x, textPosition.y, fontSize, prefixColour);

    textPosition += RIGHT * (MeasureText("#", fontSize) + margin);
    DrawText(text.c_str(), textPosition.x, textPosition.y, fontSize, colour);

    return textPosition + RIGHT * MeasureText(text.c_str(), fontSize);
  }
}