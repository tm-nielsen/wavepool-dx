#include "raylib.h"
#include <functional>
#include <vector>
#include <iostream>
#include "settings.cpp"
#include "../utils/file_utils.cpp"
#include "../ui/colour_entry_form.cpp"
#include "../ui/slider.cpp"
#include "../ui/labelled_button.cpp"
#include "../ui/toggle_button.cpp"

namespace settings {
  using namespace ui;
  using namespace utils;

  class SettingsMenu
  {
    private:
      Settings* settings;

      Slider volumeSlider;

      ColourEntryForm backgroundColourForm;

      LabelledButton resetSettingsButton;

      ToggleButton toggleFpsButton;

      std::vector<Button*> buttons;
      std::vector<CompositeUIElement*> compositeElements;

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
      void ToggleShowFps();
      void ResetSettings();
  };

  SettingsMenu::SettingsMenu(Settings* settings, vec2 screenSize, float margin, float spacing = 12):
    settings{settings}, margin{margin}, spacing{spacing}
  {
    volumeSlider = Slider();
    volumeSlider.onHandleReleased = std::bind(SetVolume, this, _1);

    backgroundColourForm = ColourEntryForm("Bg:");
    backgroundColourForm.onSubmit = std::bind(SetBackgroundColour, this, _1);

    compositeElements = {&volumeSlider, &backgroundColourForm};
    for (CompositeUIElement* elementPointer : compositeElements)
      elementPointer->BindCallbacks();


    resetSettingsButton = LabelledButton("Reset to Default");
    resetSettingsButton.onRelease = std::bind(ResetSettings, this);

    toggleFpsButton = ToggleButton();
    toggleFpsButton.onPress = std::bind(ToggleShowFps, this);

    buttons = {&resetSettingsButton, &toggleFpsButton};

    OnWindowResized(screenSize);
  }

  void SettingsMenu::LoadResources()
  {
    resetSettingsButton.LoadResources("");
    toggleFpsButton.LoadResources("resources/icons/confirm_icon.png");

    for (CompositeUIElement* elementPointer : compositeElements)
      elementPointer->LoadResources();
  }

  void SettingsMenu::UnloadResources()
  {
    toggleFpsButton.UnloadResources();

    for (CompositeUIElement* elementPointer : compositeElements)
      elementPointer->UnloadResources();
  }

  void SettingsMenu::SetStyle(Color normalColour, Color hoverColour, float thickness)
  {
    for (Button* buttonPointer : buttons)
      buttonPointer->SetStyle(normalColour, hoverColour, thickness, 15);

    for (CompositeUIElement* elementPointer : compositeElements)
      elementPointer->SetStyle(normalColour, hoverColour, thickness);
  }

  void SettingsMenu::Update()
  {
    coloursModified = false;
    windowSettingsModified = false;
    waveGridSettingsModified = false;

    vec2 mousePosition = GetMousePosition();
    for (Button* buttonPointer : buttons)
      buttonPointer->Update(mousePosition);

    for (CompositeUIElement* elementPointer : compositeElements)
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

    resetSettingsButton.SetArea(placementRect);

    placementRect.origin.y += 2 * margin + spacing;
    placementRect.size.x = placementRect.size.y;
    toggleFpsButton.SetArea(placementRect);
    // toggleFpsButton.SetArea(rect(vec2(400), vec2(40)));
  }

  void SettingsMenu::Draw() 
  {
    for (Button* buttonPointer : buttons)
      buttonPointer->Draw();

    for (CompositeUIElement* elementPointer : compositeElements)
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

  void SettingsMenu::ToggleShowFps()
  {
    std::cout << "Setting show fps to: " << toggleFpsButton.isToggled << std::endl;
  }

  void SettingsMenu::ResetSettings()
  {
    std::cout << "Resetting Settings..." << std::endl;
  }
}