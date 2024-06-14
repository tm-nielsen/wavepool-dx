#include "raylib.h"
#include <string>
#include <functional>
#include "../utils/rect.cpp"

namespace ui {
  using namespace utils;

  class TextArea
  {
    private:
      rect area;
      Color normalColour;
      Color hoveredColour;
      float borderThickness;
      float margin;
      float fontSize;

      float caretBlinkPeriod;
      float caretBlinkTimer;
      bool drawCaret;

    public:
      std::function<void(const char*)> onEdit;
      std::function<void(const char*)> onSubmit;
      std::function<void()> onFocusGained;
      std::function<void()> onFocusLost;

      std::string text;
      int maximumCharacters = 8;
      bool isHovered = false;
      bool isFocused = false;

      TextArea();
      TextArea(rect);
      TextArea(vec2, vec2);

      void SetStyle(Color, Color, float, float, float);
      void SetArea(rect);
      virtual void Update(vec2);
      virtual void ProcessTextEntry();
      virtual void NotifyEdit();
      virtual void Submit();
      void GainFocus();
      void LoseFocus();
      void Draw();
  };

  TextArea::TextArea(): area{rect()}
  {
    text = "";
    onEdit = onSubmit = [](const char* s){};
    onFocusGained = onFocusLost = [](){};
  }

  TextArea::TextArea(rect area)
  {
    TextArea();
    this->area = area;
  }

  TextArea::TextArea(vec2 origin, vec2 size)
  {
    TextArea();
    area = rect(origin, size);
  }

  
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

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
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
      drawCaret = !drawCaret;
    }

    if (IsKeyPressed(KEY_ENTER)) {
      Submit();
      return;
    }

    bool isControlPressed = IsKeyDown(KEY_LEFT_CONTROL);
    isControlPressed |= IsKeyDown(KEY_RIGHT_CONTROL);
    if (isControlPressed && IsKeyPressed(KEY_V)) {
      text = GetClipboardText();
      return;
    }
    if (isControlPressed && IsKeyPressed(KEY_C)) {
      SetClipboardText(text.c_str());
      return;
    }

    char key = GetCharPressed();
    while (key > 0) {
      bool canAppend = text.length() < maximumCharacters;
      if (key > 32 && key < 125 && canAppend) {
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

  void TextArea::NotifyEdit()
  {
    onEdit(text.c_str());
  }

  void TextArea::Submit()
  {
    onSubmit(text.c_str());
    LoseFocus();
  }

  void TextArea::GainFocus()
  {
    isFocused = true;
    drawCaret = true;
    caretBlinkTimer = 0;
    onFocusGained;
  }

  void TextArea::LoseFocus()
  {
    isFocused = false;
    drawCaret = false;
    onFocusLost();
  }

  void TextArea::Draw()
  {
    Color colour = isHovered? hoveredColour: normalColour;
    area.DrawRounded(borderThickness, colour);

    vec2 textPosition = area.origin + margin;
    DrawText(text.c_str(), textPosition.x, textPosition.y, fontSize, colour);

    if (drawCaret) {
      vec2 caretPosition = textPosition;
      caretPosition.x += MeasureText(text.c_str(), fontSize) + fontSize / 4;
      vec2 caretBottom = caretPosition + DOWN * fontSize;
      DrawLineEx(caretPosition.ToVector2(), caretBottom.ToVector2(), borderThickness, colour);
    }
  }
}