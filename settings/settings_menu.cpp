#include "raylib.h"
#include <functional>
#include <vector>
#include <iostream>
#include "settings.cpp"
#include "../utils/file_utils.cpp"
#include "../ui/colour_entry_form.cpp"
#include "../ui/slider.cpp"

namespace settings {
  using namespace ui;
  using namespace utils;

  class SettingsMenu
  {
    private:
      Settings* settings;

      Slider volumeSlider;

      ColourEntryForm backgroundColourForm;

      std::vector<CompositeUIElement*> uiElements;

      float margin;
      float spacing;

    public:
      bool coloursModified = false;
      bool windowSettingsModified = false;
      bool waveGridSettingsModified = false;

      SettingsMenu(Settings*, vec2, float, float);
      void LoadResources();
      void UnloadResources();

      void SetStyle(Color, Color, float);
      void Update();
      void OnWindowResized(vec2);
      void Draw();

      void SetVolume(float);
      void SetBackgroundColour(Color);
  };

  SettingsMenu::SettingsMenu(Settings* settings, vec2 screenSize, float margin, float spacing = 12):
    settings{settings}, margin{margin}, spacing{spacing}
  {
    backgroundColourForm = ColourEntryForm("Bg:");
    backgroundColourForm.onSubmit = std::bind(SetBackgroundColour, this, _1);

    volumeSlider = Slider();
    volumeSlider.onHandleReleased = std::bind(SetVolume, this, _1);

    uiElements = {&volumeSlider, &backgroundColourForm};
    for (CompositeUIElement* elementPointer : uiElements)
      elementPointer->BindCallbacks();

    OnWindowResized(screenSize);
  }

  void SettingsMenu::LoadResources()
  {
    for (CompositeUIElement* elementPointer : uiElements)
      elementPointer->LoadResources();
  }

  void SettingsMenu::UnloadResources()
  {
    for (CompositeUIElement* elementPointer : uiElements)
      elementPointer->UnloadResources();
  }

  void SettingsMenu::SetStyle(Color normalColour, Color hoverColour, float thickness)
  {
    for (CompositeUIElement* elementPointer : uiElements)
      elementPointer->SetStyle(normalColour, hoverColour, thickness);
  }

  void SettingsMenu::Update()
  {
    coloursModified = false;
    windowSettingsModified = false;
    waveGridSettingsModified = false;

    vec2 mousePosition = GetMousePosition();
    for (CompositeUIElement* elementPointer : uiElements)
      elementPointer->Update(mousePosition);
  }

  void SettingsMenu::OnWindowResized(vec2 screenSize)
  {
    rect placementRect = rect(vec2(2 * margin), vec2(margin / 2));
    placementRect.size.x = screenSize.x - 4 * margin;

    auto sliders = {&volumeSlider};
    for (Slider* sliderPointer : sliders)
    {
      sliderPointer->SetShape(placementRect, vec2(margin / 2, margin));
      placementRect.origin.y += 2 * margin + spacing;
    }

    auto colourEntryForms = {&backgroundColourForm};
    placementRect.size.y = 1.5 * margin;
    for (ColourEntryForm* colourFormPointer : colourEntryForms)
    {
      colourFormPointer->SetArea(placementRect);
      placementRect.origin.y += 1.5 * margin + spacing;
    }
  }

  void SettingsMenu::Draw() 
  {
    for (CompositeUIElement* elementPointer : uiElements)
      elementPointer->Draw();
  }

  void SettingsMenu::SetVolume(float sliderValue)
  {
    std::cout << "Setting Volume: " << sliderValue << "\n";
  }

  void SettingsMenu::SetBackgroundColour(Color colour)
  {
    // Color colour = utils::GetColourFromString(colourString);

    Vector4 colourVector = ColorNormalize(colour);
    std::cout << "Setting Background Colour: (";
    std::cout << colourVector.x << ", ";
    std::cout << colourVector.y << ", ";
    std::cout << colourVector.z << ", ";
    std::cout << colourVector.w << ")\n";
    coloursModified = true;
  }
}