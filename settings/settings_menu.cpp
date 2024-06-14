#include "raylib.h"
#include "../ui/button.cpp"

namespace settings {
  using namespace ui;

  class SettingsMenu
  {
    private:
      Button closeProgramButton;
      Button toggleMenuButton;

      float buttonSpacing;

    public:
      bool isOpen = false;
      bool closeButtonPressed = false;

      SettingsMenu(vec2, float, float);
      void LoadResources();
      void UnloadResources();
      void SetStyle(Color, Color, float);
      void Update();
      void Draw();
  };

  SettingsMenu::SettingsMenu(vec2 screenSize, float margin, float buttonSpacing = 12)
  {
    vec2 buttonPosition = vec2(screenSize.x - margin, 6.0);
    buttonPosition.x -= buttonSpacing;
    auto closeProgram = [this]() {
      closeButtonPressed = true;
    };
    closeProgramButton = Button(buttonPosition, vec2(margin - 12), closeProgram);

    buttonPosition.x -= margin;
    auto toggleMenu = [this]() {
      isOpen = !isOpen;
    };
    toggleMenuButton = Button(buttonPosition, vec2(margin - 12), toggleMenu);
  }

  void SettingsMenu::LoadResources()
  {
    toggleMenuButton.LoadResources("resources/icons/settings_icon.png");
    closeProgramButton.LoadResources("resources/icons/close_icon.png");
  }

  void SettingsMenu::UnloadResources()
  {
    toggleMenuButton.UnloadResources();
    closeProgramButton.UnloadResources();
  }

  void SettingsMenu::SetStyle(Color normalColour, Color hoverColour, float thickness)
  {
    toggleMenuButton.SetStyle(normalColour, hoverColour, thickness);
    closeProgramButton.SetStyle(normalColour, hoverColour, thickness);
  }

  void SettingsMenu::Update()
  {
    vec2 mousePosition = GetMousePosition();
    toggleMenuButton.Update(mousePosition);
    closeProgramButton.Update(mousePosition);
  }

  void SettingsMenu::Draw() 
  {
    toggleMenuButton.Draw();
    closeProgramButton.Draw();
  }
}