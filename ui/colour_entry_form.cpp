#include "raylib.h"
#include "../utils/file_utils.cpp"
#include "composite_ui_element.cpp"
#include "colour_input_field.cpp"
#include "button.cpp"
#include "label.cpp"

namespace ui {
  class ColourEntryForm: public CompositeUIElement
  {
    private:
      ColourEntryField textField;
      Button submitButton;
      Label label;
      rect previewArea;
      Color enteredColour;
      

    public:
      BoundColourCallback onSubmit;

      ColourEntryForm();
      ColourEntryForm(const char*);
      ColourEntryForm(const char*, rect);
      
      void LoadResources();
      void UnloadResources();
      void BindCallbacks();
      
      void SetStyle(Color, Color, float);
      void SetStyle(Color, Color, float, float, float);
      void SetArea(rect);
      void SetEnteredColour(Color);
      void Update(vec2);
      void OnTextAreaEdited(const char*);
      void OnTextAreaSubmitted(const char*);
      void OnSubmitButtonPressed();
      void Draw();
  };

  ColourEntryForm::ColourEntryForm()
  {
    textField = ColourEntryField();
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
    SetArea(area);
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
    textField.onEdit = std::bind(OnTextAreaEdited, this, _1);
    textField.onSubmit = std::bind(OnTextAreaSubmitted, this, _1);
    submitButton.onRelease = std::bind(OnSubmitButtonPressed, this);
  }

  void ColourEntryForm::SetStyle(Color normal, Color hovered, float thickness)
  {
    SetStyle(normal, hovered, thickness, thickness / 4, 0.5);
  }
  void ColourEntryForm::SetStyle(Color normal, Color hovered,
    float thickness, float fontMargin, float blinkPeriod)
  {
    borderThickness = thickness;
    textField.SetStyle(normal, hovered, thickness, fontMargin, blinkPeriod);
    submitButton.SetStyle(normal, hovered, thickness, 15);
    label.SetStyle(normal, fontMargin);
  }

  void ColourEntryForm::SetArea(rect area)
  {
    float spacing = borderThickness * 1.5;

    rect labelArea = area;
    labelArea.size.x /= 6;
    labelArea.origin.x -= borderThickness / 4;
    label.SetArea(labelArea);
    
    rect textEntryArea = area;
    textEntryArea.origin.x += labelArea.size.x;
    textEntryArea.size.x /= 2;
    textField.SetArea(textEntryArea);

    rect submitButtonRect = area;
    submitButtonRect.size.x = area.size.y;
    submitButtonRect.origin.x = area.origin.x + area.size.x - submitButtonRect.size.x;
    submitButton.SetArea(submitButtonRect);

    rect remainingArea = textEntryArea;
    remainingArea.origin.x += textEntryArea.size.x + spacing;
    remainingArea.size.x = area.size.x / 3 - (submitButtonRect.size.x + 2 * spacing);
    previewArea = remainingArea;
  }

  void ColourEntryForm::SetEnteredColour(Color colour)
  {
    enteredColour = colour;
    std::string colourString = utils::GetString(colour);
    textField.SetText(colourString);
  }

  void ColourEntryForm::Update(vec2 mousePosition)
  {
    textField.Update(mousePosition);
    submitButton.Update(mousePosition);
  }

  void ColourEntryForm::OnTextAreaEdited(const char* hexString)
  {
    enteredColour = GetColourFromString(hexString);
  }

  void ColourEntryForm::OnTextAreaSubmitted(const char* hexString)
  {
    enteredColour = GetColourFromString(hexString);
    if (onSubmit) onSubmit(enteredColour);
  }

  void ColourEntryForm::OnSubmitButtonPressed()
  {
    if (onSubmit) onSubmit(enteredColour);
  }

  void ColourEntryForm::Draw()
  {
    label.Draw();
    previewArea.DrawRoundedFilled(enteredColour);
    textField.Draw();
    submitButton.Draw();
  }
}