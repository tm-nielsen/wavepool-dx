#include "raylib.h"
#include <functional>
#include <vector>
#include <iostream>
#include "settings.cpp"
#include "../utils/file_utils.cpp"
#include "../ui/colour_entry_area.cpp"
#include "../ui/slider.cpp"

namespace settings {
  using namespace ui;
  using namespace utils;

  class SettingsMenu
  {
    private:
      Settings* settings;

      Slider volumeSlider;

      ColourEntryArea backgroundColourTextArea;

      std::vector<Slider*> sliders;
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

      void SetVolume(float);
      void SetBackgroundColour(const char*);
  };

  SettingsMenu::SettingsMenu(Settings* settings, vec2 screenSize, float margin, float spacing = 12):
    settings{settings}, margin{margin}, spacing{spacing}
  {
    backgroundColourTextArea = ColourEntryArea();
    backgroundColourTextArea.onSubmit = std::bind(SetBackgroundColour, this, _1);

    volumeSlider = Slider();
    volumeSlider.onHandleReleased = std::bind(SetVolume, this, _1);

    sliders = {&volumeSlider};
    for (Slider* sliderPointer : sliders) sliderPointer->BindHandleCallbacks();

    textAreas = {&backgroundColourTextArea};
    OnWindowResized(screenSize);
  }

  void SettingsMenu::SetStyle(Color normalColour, Color hoverColour, float thickness)
  {
    for (Slider* sliderPointer : sliders)
      sliderPointer->SetStyle(normalColour, hoverColour, thickness);
    for (TextArea* textAreaPointer : textAreas)
      textAreaPointer->SetStyle(normalColour, hoverColour, thickness);
  }

  void SettingsMenu::Update()
  {
    coloursModified = false;
    windowSettingsModified = false;
    waveGridSettingsModified = false;

    vec2 mousePosition = GetMousePosition();
    for (Slider* sliderPointer : sliders)
      sliderPointer->Update(mousePosition);
    for (TextArea* textAreaPointer : textAreas)
      textAreaPointer->Update(mousePosition);
  }

  void SettingsMenu::OnWindowResized(vec2 screenSize)
  {
    rect placementRect = rect(vec2(2 * margin), vec2(margin / 2));
    placementRect.size.x = screenSize.x - 4 * margin;

    for (Slider* sliderPointer : sliders)
    {
      sliderPointer->SetShape(placementRect, vec2(margin / 2, margin));
      placementRect.origin.y += 2 * margin + spacing;
    }

    placementRect.size.y = 2 * margin;
    for (TextArea* textAreaPointer : textAreas)
    {
      textAreaPointer->SetArea(placementRect);
      placementRect.origin.y += 2 * margin + spacing;
    }
  }

  void SettingsMenu::Draw() 
  {
    for (Slider* sliderPointer : sliders)
      sliderPointer->Draw();
    for (TextArea* textAreaPointer : textAreas)
      textAreaPointer->Draw();
  }

  void SettingsMenu::SetVolume(float sliderValue)
  {
    std::cout << "Setting Volume: " << sliderValue << "\n";
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