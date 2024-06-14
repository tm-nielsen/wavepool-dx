#include "raylib.h"
#include <functional>
#include <vector>
#include <iostream>
#include "settings.cpp"
#include "../utils/file_utils.cpp"
#include "../ui/colour_entry_area.cpp"

namespace settings {
  using namespace ui;
  using namespace utils;

  class SettingsMenu
  {
    private:
      Settings* settings;
      ColourEntryArea backgroundColourTextArea;

      std::vector<TextArea*> textAreas;

      float margin;
      float spacing;

    public:
      bool coloursModified = false;
      bool windowSettingsModified = false;
      bool waveGridSettingsModified = false;

      SettingsMenu(Settings*, vec2, float, float);
      void SetStyle(Color, Color, float);
      void Update();
      void OnWindowResized(vec2);
      void Draw();

      void SetBackgroundColour(const char*);
  };

  SettingsMenu::SettingsMenu(Settings* settings, vec2 screenSize, float margin, float spacing = 12):
    settings{settings}, margin{margin}, spacing{spacing}
  {
    backgroundColourTextArea = ColourEntryArea();
    backgroundColourTextArea.onSubmit = [this](const char* s){SetBackgroundColour(s);};

    textAreas = {&backgroundColourTextArea};
    OnWindowResized(screenSize);
  }

  void SettingsMenu::SetStyle(Color normalColour, Color hoverColour, float thickness)
  {
    for (TextArea* textAreaPointer : textAreas)
      textAreaPointer->SetStyle(normalColour, hoverColour, thickness);
  }

  void SettingsMenu::Update()
  {
    coloursModified = false;
    windowSettingsModified = false;
    waveGridSettingsModified = false;

    vec2 mousePosition = GetMousePosition();
    for (TextArea* textAreaPointer : textAreas)
      textAreaPointer->Update(mousePosition);
  }

  void SettingsMenu::OnWindowResized(vec2 screenSize)
  {
    rect placementRect = rect(vec2(2 * margin), vec2(margin - 12));
    placementRect.origin = vec2(2 * margin);
    placementRect.size = vec2(screenSize.x - 4 * margin, 2 * margin);

    for (TextArea* textAreaPointer : textAreas)
    {
      textAreaPointer->SetArea(placementRect);
      placementRect.origin.y += 2 * margin + spacing;
    }
  }

  void SettingsMenu::Draw() 
  {
    for (TextArea* textAreaPointer : textAreas)
      textAreaPointer->Draw();
  }

  void SettingsMenu::SetBackgroundColour(const char* colourString)
  {
    Color colour = utils::GetColourFromString(colourString);

    Vector4 colourVector = ColorNormalize(colour);
    std::cout << "Setting Background Colour: (";
    std::cout << colourVector.x << ", ";
    std::cout << colourVector.y << ", ";
    std::cout << colourVector.z << ", ";
    std::cout << colourVector.w << ")\n";
    coloursModified = true;
  }
}