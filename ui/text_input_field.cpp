#include "raylib.h"
#include <string>
#include "hoverable_ui_element.cpp"

namespace ui {
#ifndef TEXT_INPUT_FIELD
#define TEXT_INPUT_FIELD
  class TextInputField: public HoverableUIElement
  {
    private:
      float caretBlinkPeriod;
      float caretBlinkTimer;
      bool shouldDrawCaret = false;

    protected:
      float margin;
      float fontSize;

    public:
      BoundStringCallback onEdit;
      BoundStringCallback onSubmit;
      BoundCallback onFocusGained;
      BoundCallback onFocusLost;

      std::string text = "";
      int maximumCharacters = 8;
      bool isFocused = false;

      TextInputField(rect);
      TextInputField(vec2, vec2);

      void SetStyle(Color, Color, float, float, float);
      void SetArea(rect);
      void Update(vec2);
      void ProcessTextEntry();
      virtual void PasteFromClipboard();
      virtual void CopyToClipboard();
      virtual bool IsCharacterValid(char);
      void NotifyEdit();
      void Submit();
      void GainFocus();
      void LoseFocus();
      void Draw();
      virtual vec2 DrawEnteredText(Color);
      void DrawCaret(vec2, Color);
  };

  TextInputField::TextInputField(rect textArea = rect()) { SetArea(textArea); }
  TextInputField::TextInputField(vec2 origin, vec2 size) { SetArea(rect(origin, size)); }

  
  void TextInputField::SetStyle(Color normal, Color hovered,
    float thickness, float fontMargin = 12, float blinkPeriod = 0.5)
  {
    HoverableUIElement::SetStyle(normal, hovered, thickness);
    margin = fontMargin;
    caretBlinkPeriod = blinkPeriod;
    fontSize = area.size.y - 2 * margin;
  }

  void TextInputField::SetArea(rect area)
  {
    UIElement::SetArea(area);
    fontSize = area.size.y - 2 * margin;
  }

  void TextInputField::Update(vec2 mousePosition)
  {
    HoverableUIElement::Update(mousePosition);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
      if (!isFocused && isHovered)
        GainFocus();
      else if (isFocused && !isHovered)
        LoseFocus();
    }

    if (isFocused)
      ProcessTextEntry();
  }

  void TextInputField::ProcessTextEntry()
  {
    caretBlinkTimer += GetFrameTime();
    if (caretBlinkTimer > caretBlinkPeriod) {
      caretBlinkTimer -= caretBlinkPeriod;
      shouldDrawCaret = !shouldDrawCaret;
    }

    if (IsKeyPressed(KEY_ENTER)) {
      Submit();
      return;
    }

    bool isControlPressed = IsKeyDown(KEY_LEFT_CONTROL);
    isControlPressed |= IsKeyDown(KEY_RIGHT_CONTROL);
    if (isControlPressed && IsKeyPressed(KEY_V)) {
      PasteFromClipboard();
      return;
    }
    if (isControlPressed && IsKeyPressed(KEY_C)) {
      CopyToClipboard();
      return;
    }

    char key = GetCharPressed();
    while (key > 0) {
      bool canAppend = text.length() < maximumCharacters;
      if (IsCharacterValid(key) && canAppend) {
        text += key;
        NotifyEdit();
      }
      key = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE)) {
      text = text.substr(0, text.length() - 1);
      NotifyEdit();
    }
    if (IsKeyPressed(KEY_DELETE)) {
      text = "";
      NotifyEdit();
    }
  }

  void TextInputField::PasteFromClipboard()
  {
    text = GetClipboardText();
  }

  void TextInputField::CopyToClipboard()
  {
    SetClipboardText(text.c_str());
  }

  bool TextInputField::IsCharacterValid(char c)
  {
    return c > 32 && c < 125;
  }

  void TextInputField::NotifyEdit()
  {
    if (onEdit) onEdit(text.c_str());
  }

  void TextInputField::Submit()
  {
    if (onSubmit) onSubmit(text.c_str());
    LoseFocus();
  }

  void TextInputField::GainFocus()
  {
    isFocused = true;
    shouldDrawCaret = true;
    caretBlinkTimer = 0;
    if (onFocusGained) onFocusGained();;
  }

  void TextInputField::LoseFocus()
  {
    isFocused = false;
    shouldDrawCaret = false;
    if (onFocusLost) onFocusLost();
  }

  void TextInputField::Draw()
  {
    Color colour = isHovered? hoveredColour: normalColour;
    if (isFocused)
      colour = hoveredColour;
    Color borderColour = isFocused? normalColour: colour;
    area.DrawRounded(borderThickness, borderColour);

    vec2 textEndPosition = DrawEnteredText(colour);
    DrawCaret(textEndPosition, colour);
  }

  vec2 TextInputField::DrawEnteredText(Color colour)
  {
    vec2 textPosition = area.origin + margin;
    DrawText(text.c_str(), textPosition.x, textPosition.y, fontSize, colour);
    return textPosition + RIGHT * MeasureText(text.c_str(), fontSize);
  }

  void TextInputField::DrawCaret(vec2 textEndPosition, Color colour)
  {
    if (shouldDrawCaret) {
      vec2 caretPosition = textEndPosition + RIGHT * margin;
      vec2 caretBottom = caretPosition + DOWN * fontSize;
      DrawLineEx(caretPosition.ToVector2(), caretBottom.ToVector2(), borderThickness, colour);
    }
  }
#endif
}