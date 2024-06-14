#include "raylib.h"
#include "../ui/button.cpp"
#include "../ui/drag_button.cpp"

namespace window_management {
  using namespace ui;

  class WindowManager
  {
    private:
      Button closeProgramButton;
      Button toggleMenuButton;
      DragButton moveButton;

      Button* buttons [3];

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
    buttons[0] = &closeProgramButton;

    buttonRect.origin.x -= margin;
    auto toggleMenu = [this]() {
      settingsMenuIsOpen = !settingsMenuIsOpen;
    };
    toggleMenuButton = Button(buttonRect);
    toggleMenuButton.onPress = toggleMenu;
    buttons[1] = &toggleMenuButton;

    buttonRect.origin.x -= margin;
    auto moveWindow = [](vec2 offset) {
      vec2 windowPosition = GetWindowPosition();
      windowPosition += offset;
      SetWindowPosition(windowPosition.x, windowPosition.y);
    };
    moveButton = DragButton(buttonRect, true);
    moveButton.onDrag = moveWindow;
    buttons[2] = &moveButton;
  }

  void WindowManager::LoadResources()
  {
    closeProgramButton.LoadResources("resources/icons/close_icon.png");
    toggleMenuButton.LoadResources("resources/icons/settings_icon.png");
    moveButton.LoadResources("resources/icons/move_icon.png");
  }

  void WindowManager::UnloadResources()
  {
    for (Button* buttonPointer : buttons)
      buttonPointer->UnloadResources();
  }

  void WindowManager::SetStyle(Color normalColour, Color hoverColour, float thickness)
  {
    for (Button* buttonPointer : buttons)
      buttonPointer->SetStyle(normalColour, hoverColour, thickness);
  }

  void WindowManager::Update()
  {
    vec2 mousePosition = GetMousePosition();
    for (Button* buttonPointer : buttons)
      buttonPointer->Update(mousePosition);
  }

  void WindowManager::Draw() 
  {
    for (Button* buttonPointer : buttons)
      buttonPointer->Draw();
  }
}