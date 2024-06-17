#include "raylib.h"
#include "colour_entry_area.cpp"
#include "../utils/file_utils.cpp"
#include "button.cpp"
#include "label.cpp"

namespace ui {
  class ColourEntryForm
  {
    private:
      ColourEntryArea textArea;
      Button submitButton;
      Label label;
      rect previewArea;
      Color colour;
      

    public:
      BoundColourCallback onSubmit;

      ColourEntryForm();
      ColourEntryForm(const char*);
      ColourEntryForm(const char*, rect);
      
      void LoadResources();
      void UnloadResources();
      void BindCallbacks();
      
      void SetStyle(Color, Color, float, float, float);
      void SetArea(rect, float);
      void Update(vec2);
      void OnTextAreaEdited(const char*);
      void OnTextAreaSubmitted(const char*);
      void OnSubmitButtonPressed();
      void Draw();
  };

  ColourEntryForm::ColourEntryForm()
  {
    textArea = ColourEntryArea();
    submitButton = Button();
    label = Label();
  }

  ColourEntryForm::ColourEntryForm(const char* labelText)
  {
    ColourEntryForm();
    label.SetText(labelText);
  }

  ColourEntryForm::ColourEntryForm(const char* labelText, rect area)
  {
    ColourEntryForm();
    label.SetText(labelText);
    SetArea(area, 12);
  }


  void ColourEntryForm::LoadResources()
  {
    submitButton.LoadResources("resources/icons/confirm_icon.png");
  }
  void ColourEntryForm::UnloadResources()
  {
    submitButton.UnloadResources();
  }

  void ColourEntryForm::BindCallbacks()
  {
    textArea.onEdit = std::bind(OnTextAreaEdited, this, _1);
    textArea.onSubmit = std::bind(OnTextAreaSubmitted, this, _1);
    submitButton.onPress = std::bind(OnSubmitButtonPressed, this);
  }

  void ColourEntryForm::SetStyle(Color normal, Color hovered,
    float thickness, float fontMargin = 12, float blinkPeriod = 0.5)
  {
    textArea.SetStyle(normal, hovered, thickness, fontMargin, blinkPeriod);
    submitButton.SetStyle(normal, hovered, thickness, 15);
    label.SetStyle(normal, fontMargin);
  }

  void ColourEntryForm::SetArea(rect area, float spacing = 12)
  {
    rect labelArea = area;
    labelArea.size.x /= 6;
    labelArea.origin.x -= spacing;
    label.SetArea(labelArea);
    
    rect textEntryArea = area;
    textEntryArea.origin.x += labelArea.size.x;
    textEntryArea.size.x /= 2;
    textArea.SetArea(textEntryArea);

    rect submitButtonRect = area;
    submitButtonRect.size.x = area.size.y;
    submitButtonRect.size /= 2;
    submitButtonRect.origin.y += area.size.y / 4;
    submitButtonRect.origin.x = area.origin.x + area.size.x - submitButtonRect.size.x;
    submitButton.SetArea(submitButtonRect);

    rect remainingArea = textEntryArea;
    remainingArea.origin.x += textEntryArea.size.x + spacing;
    remainingArea.size.x = area.size.x / 3 - (submitButtonRect.size.x + 2 * spacing);
    previewArea = remainingArea;
  }

  void ColourEntryForm::Update(vec2 mousePosition)
  {
    textArea.Update(mousePosition);
    submitButton.Update(mousePosition);
  }

  void ColourEntryForm::OnTextAreaEdited(const char* hexString)
  {
    colour = GetColourFromString(hexString);
  }

  void ColourEntryForm::OnTextAreaSubmitted(const char* hexString)
  {
    colour = GetColourFromString(hexString);
    if (onSubmit) onSubmit(colour);
  }

  void ColourEntryForm::OnSubmitButtonPressed()
  {
    if (onSubmit) onSubmit(colour);
  }

  void ColourEntryForm::Draw()
  {
    label.Draw();
    previewArea.DrawRoundedFilled(colour);
    textArea.Draw();
    submitButton.Draw();
  }
}