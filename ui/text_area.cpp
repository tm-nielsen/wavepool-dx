#include "raylib.h"
#include <string>
#include "ui_callback_types.cpp"
#include "../utils/rect.cpp"

namespace ui {
#ifndef TEXT_AREA
#define TEXT_AREA
  class TextArea
  {
    private:
      float borderThickness;
      float caretBlinkPeriod;
      float caretBlinkTimer;
      bool shouldDrawCaret;

    protected:
      rect area;
      Color normalColour;
      Color hoveredColour;
      float margin;
      float fontSize;

    public:
      BoundStringCallback onEdit;
      BoundStringCallback onSubmit;
      BoundCallback onFocusGained;
      BoundCallback onFocusLost;

      std::string text = "";
      int maximumCharacters = 8;
      bool isHovered = false;
      bool isFocused = false;

      TextArea();
      TextArea(rect);
      TextArea(vec2, vec2);

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

  TextArea::TextArea(): area{rect()} {}
  TextArea::TextArea(rect area) { SetArea(area); }
  TextArea::TextArea(vec2 origin, vec2 size) { SetArea(rect(origin, size)); }

  
  void TextArea::SetStyle(Color normal, Color hovered,
    float thickness, float fontMargin = 12, float blinkPeriod = 0.5)
  {
    normalColour = normal;
    hoveredColour = hovered;
    borderThickness = thickness;
    margin = fontMargin;
    caretBlinkPeriod = blinkPeriod;
    fontSize = area.size.y - 2 * margin;
  }

  void TextArea::SetArea(rect area)
  {
    this->area = area;
    fontSize = area.size.y - 2 * margin;
  }

  void TextArea::Update(vec2 mousePosition)
  {
    bool mouseContained = area.ContainsPoint(mousePosition);
    isHovered = mouseContained;

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
      if (!isFocused && mouseContained)
        GainFocus();
      else if (isFocused && !mouseContained)
        LoseFocus();
    }

    if (isFocused)
      ProcessTextEntry();
  }

  void TextArea::ProcessTextEntry()
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

  void TextArea::PasteFromClipboard()
  {
    text = GetClipboardText();
  }

  void TextArea::CopyToClipboard()
  {
    SetClipboardText(text.c_str());
  }

  bool TextArea::IsCharacterValid(char c)
  {
    return c > 32 && c < 125;
  }

  void TextArea::NotifyEdit()
  {
    if (onEdit) onEdit(text.c_str());
  }

  void TextArea::Submit()
  {
    if (onSubmit) onSubmit(text.c_str());
    LoseFocus();
  }

  void TextArea::GainFocus()
  {
    isFocused = true;
    shouldDrawCaret = true;
    caretBlinkTimer = 0;
    if (onFocusGained) onFocusGained();;
  }

  void TextArea::LoseFocus()
  {
    isFocused = false;
    shouldDrawCaret = false;
    if (onFocusLost) onFocusLost();
  }

  void TextArea::Draw()
  {
    Color colour = isHovered? hoveredColour: normalColour;
    if (isFocused)
      colour = hoveredColour;
    Color borderColour = isFocused? normalColour: colour;
    area.DrawRounded(borderThickness, borderColour);

    vec2 textEndPosition = DrawEnteredText(colour);
    DrawCaret(textEndPosition, colour);
  }

  vec2 TextArea::DrawEnteredText(Color colour)
  {
    vec2 textPosition = area.origin + margin;
    DrawText(text.c_str(), textPosition.x, textPosition.y, fontSize, colour);
    return textPosition + RIGHT * MeasureText(text.c_str(), fontSize);
  }

  void TextArea::DrawCaret(vec2 textEndPosition, Color colour)
  {
    if (shouldDrawCaret) {
      vec2 caretPosition = textEndPosition + RIGHT * margin;
      vec2 caretBottom = caretPosition + DOWN * fontSize;
      DrawLineEx(caretPosition.ToVector2(), caretBottom.ToVector2(), borderThickness, colour);
    }
  }
#endif
}