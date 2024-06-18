#include "raylib.h"
#include <functional>
#include <vector>
#include <iostream>
#include "settings.cpp"
#include "../utils/file_utils.cpp"
#include "../ui/colour_entry_form.cpp"
#include "../ui/labelled_slider.cpp"
#include "../ui/labelled_toggle_button.cpp"
#include "../ui/text_button.cpp"

namespace settings {
  using namespace ui;
  using namespace utils;

  class SettingsMenu
  {
    private:
      Settings* settings;
      std::vector<UIElement*> elements;

      LabelledSlider volumeSlider;
      LabelledSlider dotSizeSlider;
      LabelledSlider dotSpacingSlider;

      ColourEntryForm mainColourForm;
      ColourEntryForm backgroundColourForm;
      ColourEntryForm accentColourForm;

      LabelledSlider marginSlider;
      LabelledSlider thicknessSlider;

      LabelledToggleButton toggleFpsButton;
      TextButton resetSettingsButton;

    public:
      bool styleSettingsModifed = false;
      bool marginSettingModified = false;
      bool waveGridSettingsModified = false;
      bool windowResized = false;

      SettingsMenu(Settings*);
      void LoadResources();
      void UnloadResources();

      void SetStyle(Color, Color, float);
      void Update();
      void OnWindowResized(vec2, float);
      void Draw();

      void ApplyLoadedSettings();

      void SetVolume(float);
      void SetGridRadius(float);
      void SetGridSpacing(float);
      void SetMainColour(Color);
      void SetBackgroundColour(Color);
      void SetAccentColour(Color);
      void SetMargin(float);
      void SetBorderThickness(float);
      void ToggleShowFps(bool);
      void ResetSettings();
  };

  SettingsMenu::SettingsMenu(Settings* settings)
  {
    this->settings = settings;

    volumeSlider = LabelledSlider("Volume");
    volumeSlider.onHandleReleased = std::bind(SetVolume, this, _1);
    elements.push_back(&volumeSlider);

    dotSizeSlider = LabelledSlider("Dot Size");
    dotSizeSlider.onHandleReleased = std::bind(SetGridRadius, this, _1);
    elements.push_back(&dotSizeSlider);

    dotSpacingSlider = LabelledSlider("Spacing");
    dotSpacingSlider.onHandleReleased = std::bind(SetGridSpacing, this, _1);
    elements.push_back(&dotSpacingSlider);


    mainColourForm = ColourEntryForm("Main:");
    mainColourForm.onSubmit = std::bind(SetMainColour, this, _1);
    elements.push_back(&mainColourForm);

    backgroundColourForm = ColourEntryForm("Bg:");
    backgroundColourForm.onSubmit = std::bind(SetBackgroundColour, this, _1);
    elements.push_back(&backgroundColourForm);

    accentColourForm = ColourEntryForm("Acc:");
    accentColourForm.onSubmit = std::bind(SetAccentColour, this, _1);
    elements.push_back(&accentColourForm);


    marginSlider = LabelledSlider("Margin");
    marginSlider.onHandleReleased = std::bind(SetMargin, this, _1);
    elements.push_back(&marginSlider);

    thicknessSlider = LabelledSlider("Line W");
    thicknessSlider.onHandleReleased = std::bind(SetBorderThickness, this, _1);
    elements.push_back(&thicknessSlider);


    toggleFpsButton = LabelledToggleButton("Show FPS");
    toggleFpsButton.onToggle = std::bind(ToggleShowFps, this, _1);
    elements.push_back(&toggleFpsButton);


    std::vector<CompositeUIElement*> compositeElements = {
      &volumeSlider, &toggleFpsButton,
      &dotSizeSlider, &dotSpacingSlider,
      &marginSlider, &thicknessSlider,
      &mainColourForm, &backgroundColourForm, &accentColourForm
    };
    for (CompositeUIElement* elementPointer : compositeElements)
      elementPointer->BindCallbacks();


    resetSettingsButton = TextButton("Reset to Default");
    resetSettingsButton.onRelease = std::bind(ResetSettings, this);
    elements.push_back(&resetSettingsButton);

    ApplyLoadedSettings();
  }

  void SettingsMenu::LoadResources()
  {
    toggleFpsButton.LoadResources();
    mainColourForm.LoadResources();
    backgroundColourForm.LoadResources();
    accentColourForm.LoadResources();
  }

  void SettingsMenu::UnloadResources()
  {
    toggleFpsButton.UnloadResources();
    mainColourForm.UnloadResources();
    backgroundColourForm.UnloadResources();
    accentColourForm.UnloadResources();
  }

  void SettingsMenu::SetStyle(Color normalColour, Color hoverColour, float thickness)
  {
    for (UIElement* elementPointer : elements)
      elementPointer->SetStyle(normalColour, hoverColour, thickness);
  }

  void SettingsMenu::Update()
  {
    styleSettingsModifed = false;
    marginSettingModified = false;
    waveGridSettingsModified = false;
    windowResized = false;

    vec2 mousePosition = GetMousePosition();
    for (UIElement* elementPointer : elements)
      elementPointer->Update(mousePosition);
  }

  void SettingsMenu::OnWindowResized(vec2 screenSize, float margin)
  {
    settings->windowWidth = screenSize.x;
    settings->windowHeight = screenSize.y;
    settings->SaveToFile();

    margin *= 1.4;
    vec2 menuSize = screenSize - 2 * margin;
    if (menuSize.y > menuSize.x)
      menuSize.y = menuSize.x;
    if (menuSize.x > menuSize.y * 1.4)
      menuSize.x = menuSize.y * 1.4;

    vec2 elementSize = vec2();
    elementSize.x = menuSize.x;
    elementSize.y = menuSize.y / (elements.size() + 4);

    rect placementRect = rect(vec2(margin), elementSize);
    placementRect.origin += (screenSize - menuSize) / 2 - margin;
    vec2 placementOffset = DOWN * (menuSize.y - elementSize.y) / (elements.size() - 1);

    for (UIElement* elementPointer : elements)
    {
      elementPointer->SetArea(placementRect);
      placementRect.origin += placementOffset;
    }
  }

  void SettingsMenu::Draw() 
  {
    for (UIElement* elementPointer : elements)
      elementPointer->Draw();
  }

  void SettingsMenu::ApplyLoadedSettings()
  {
    volumeSlider.SetValue(settings->volume);

    float sliderValue;
    sliderValue = Slider::GetNormalizedValueFromRange(settings->dotSize, 1, 20);
    dotSizeSlider.SetValue(sliderValue);
    sliderValue = Slider::GetNormalizedValueFromRange(settings->dotSpacing, 4, 40);
    dotSpacingSlider.SetValue(sliderValue);
    
    mainColourForm.SetEnteredColour(settings->mainColour);
    backgroundColourForm.SetEnteredColour(settings->backgroundColour);
    accentColourForm.SetEnteredColour(settings->accentColour);
    
    sliderValue = Slider::GetNormalizedValueFromRange(settings->margin, 10, 100);
    marginSlider.SetValue(sliderValue);
    sliderValue = Slider::GetNormalizedValueFromRange(settings->lineThickness, 0, 10);
    thicknessSlider.SetValue(sliderValue);

    toggleFpsButton.SetIsToggled(settings->showFps);
  }

  void SettingsMenu::SetVolume(float sliderValue)
  {
    settings->volume = sliderValue;
    settings->SaveToFile();

    SetMasterVolume(sliderValue);
  }

  void SettingsMenu::SetGridRadius(float sliderValue)
  {
    float radius = Slider::MapNormalizedValueToRange(sliderValue, 1, 20);
    settings->dotSize = radius;
    settings->SaveToFile();

    waveGridSettingsModified = true;
  }

  void SettingsMenu::SetGridSpacing(float sliderValue)
  {
    float spacing = Slider::MapNormalizedValueToRange(sliderValue, 4, 40);
    settings->dotSpacing = spacing;
    settings->SaveToFile();
    
    waveGridSettingsModified = true;
  }

  void SettingsMenu::SetMainColour(Color colour)
  {
    settings->mainColour = colour;
    settings->SaveToFile();

    styleSettingsModifed = true;
  }

  void SettingsMenu::SetBackgroundColour(Color colour)
  {
    settings->backgroundColour = colour;
    settings->SaveToFile();

    styleSettingsModifed = true;
  }

  void SettingsMenu::SetAccentColour(Color colour)
  {
    settings->accentColour = colour;
    settings->SaveToFile();

    styleSettingsModifed = true;
  }

  void SettingsMenu::SetMargin(float sliderValue)
  {
    settings->margin = Slider::MapNormalizedValueToRange(sliderValue, 10, 100);
    settings->SaveToFile();

    marginSettingModified = true;
  }

  void SettingsMenu::SetBorderThickness(float sliderValue)
  {
    settings->lineThickness = Slider::MapNormalizedValueToRange(sliderValue, 0, 10);
    settings->SaveToFile();

    styleSettingsModifed = true;
  }

  void SettingsMenu::ToggleShowFps(bool isToggled)
  {
    settings->showFps = isToggled;
    settings->SaveToFile();
  }

  void SettingsMenu::ResetSettings()
  {
    *settings = Settings();
    settings->SaveToFile();
    ApplyLoadedSettings();

    styleSettingsModifed = true;
    marginSettingModified = true;
    waveGridSettingsModified = true;

    SetWindowSize(settings->windowWidth, settings->windowHeight);
    windowResized = true;
  }
}