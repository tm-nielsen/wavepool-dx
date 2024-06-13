#include "raylib.h"
#include "../ui/button.cpp"

namespace settings {
  using namespace ui;

  class SettingsMenu
  {
    private:
      Button openButton;

    public:
      bool isOpen = false;

      SettingsMenu(vec2, float);
      void LoadResources();
      void UnloadResources();
      void SetStyle(Color, Color, float);
      void Update();
      void Draw();
  };

  SettingsMenu::SettingsMenu(vec2 screenSize, float margin)
  {
    auto onOpenButtonClicked = [this]() {
      isOpen = !isOpen;
    };
    openButton = Button(vec2(screenSize.x - margin, 0.0), vec2(margin), onOpenButtonClicked);
  }

  void SettingsMenu::LoadResources()
  {
    openButton.LoadResources("resources/icons/settings_icon.png");
  }

  void SettingsMenu::UnloadResources()
  {
    openButton.UnloadResources();
  }

  void SettingsMenu::SetStyle(Color normalColour, Color hoverColour, float thickness)
  {
    openButton.SetStyle(normalColour, hoverColour, thickness);
  }

  void SettingsMenu::Update()
  {
    vec2 mousePosition = GetMousePosition();
    openButton.Update(mousePosition);
  }

  void SettingsMenu::Draw() 
  {
    openButton.Draw();
  }
}