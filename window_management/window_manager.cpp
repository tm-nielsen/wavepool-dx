#include "raylib.h"
#include "../ui/button.cpp"

namespace window_management {
  using namespace ui;

  class WindowManager
  {
    private:
      Button closeProgramButton;
      Button toggleMenuButton;

      float buttonSpacing;

    public:
      bool shouldExitProgram = false;
      bool settingsMenuIsOpen = false;

      WindowManager(vec2, float, float);
      void LoadResources();
      void UnloadResources();
      void SetStyle(Color, Color, float);
      void Update();
      void Draw();
  };

  WindowManager::WindowManager(vec2 screenSize, float margin, float buttonSpacing = 12)
  {
    rect buttonRect = rect(vec2(screenSize.x - margin, 6.0), vec2(margin - 12));
    buttonRect.origin.x -= buttonSpacing;
    auto closeProgram = [this]() {
      shouldExitProgram = true;
    };
    closeProgramButton = Button(buttonRect);
    closeProgramButton.onRelease = closeProgram;

    buttonRect.origin.x -= margin;
    auto toggleMenu = [this]() {
      settingsMenuIsOpen = !settingsMenuIsOpen;
    };
    toggleMenuButton = Button(buttonRect);
    toggleMenuButton.onPress = toggleMenu;
  }

  void WindowManager::LoadResources()
  {
    closeProgramButton.LoadResources("resources/icons/close_icon.png");
    toggleMenuButton.LoadResources("resources/icons/settings_icon.png");
  }

  void WindowManager::UnloadResources()
  {
    closeProgramButton.UnloadResources();
    toggleMenuButton.UnloadResources();
  }

  void WindowManager::SetStyle(Color normalColour, Color hoverColour, float thickness)
  {
    closeProgramButton.SetStyle(normalColour, hoverColour, thickness);
    toggleMenuButton.SetStyle(normalColour, hoverColour, thickness);
  }

  void WindowManager::Update()
  {
    vec2 mousePosition = GetMousePosition();
    closeProgramButton.Update(mousePosition);
    toggleMenuButton.Update(mousePosition);
  }

  void WindowManager::Draw() 
  {
    closeProgramButton.Draw();
    toggleMenuButton.Draw();
  }
}