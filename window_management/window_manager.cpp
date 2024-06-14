#include "raylib.h"
#include <functional>
#include <vector>
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
      DragButton resizeButton;

      std::vector<Button*> buttons;

      vec2 minimumSize;
      float margin;
      float buttonSpacing;

    public:
      bool shouldExitProgram = false;
      bool settingsMenuIsOpen = false;
      bool windowResized = false;

      WindowManager(vec2, vec2, float, float);
      void LoadResources();
      void UnloadResources();
      void SetStyle(Color, Color, float);
      void Update();
      void OnWindowResized(vec2);
      void Draw();
  };

  WindowManager::WindowManager(vec2 size, vec2 minimumSize, float margin, float buttonSpacing = 12):
    minimumSize{minimumSize}, margin{margin}, buttonSpacing{buttonSpacing}
  {
    auto closeProgram = [this]() {
      shouldExitProgram = true;
    };
    closeProgramButton = Button();
    closeProgramButton.onRelease = closeProgram;
    buttons.push_back(&closeProgramButton);

    auto toggleMenu = [this]() {
      settingsMenuIsOpen = !settingsMenuIsOpen;
    };
    toggleMenuButton = Button();
    toggleMenuButton.onPress = toggleMenu;
    buttons.push_back(&toggleMenuButton);

    auto moveWindow = [](vec2 offset) {
      vec2 windowPosition = GetWindowPosition();
      windowPosition += offset;
      SetWindowPosition(windowPosition.x, windowPosition.y);
    };
    moveButton = DragButton(vec2(-1, -1));
    moveButton.onDrag = moveWindow;
    moveButton.holdKey = KEY_SPACE;
    buttons.push_back(&moveButton);

    auto resizeWindow = [this, minimumSize](vec2 offset) {
      vec2 windowSize = vec2(GetScreenWidth(), GetScreenHeight());
      float currentWindowHeight = windowSize.y;
      vec2 windowPosition = GetWindowPosition();
      
      offset.y *= -1;
      windowSize += offset;
      windowSize = windowSize.ClampLower(minimumSize);
      windowPosition.y -= windowSize.y - currentWindowHeight;

      SetWindowSize(windowSize.x, windowSize.y);
      SetWindowPosition(windowPosition.x, windowPosition.y);

      windowResized = true;
      OnWindowResized(windowSize);
    };
    resizeButton = DragButton(UP);
    resizeButton.onDrag = resizeWindow;
    resizeButton.holdKey = KEY_R;
    buttons.push_back(&resizeButton);

    OnWindowResized(size);
  }

  void WindowManager::LoadResources()
  {
    closeProgramButton.LoadResources("resources/icons/close_icon.png");
    toggleMenuButton.LoadResources("resources/icons/settings_icon.png");
    moveButton.LoadResources("resources/icons/move_icon.png");
    resizeButton.LoadResources("resources/icons/scale_icon.png");
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
    windowResized = false;
    vec2 mousePosition = GetMousePosition();
    for (Button* buttonPointer : buttons)
      buttonPointer->Update(mousePosition);
  }

  void WindowManager::OnWindowResized(vec2 screenSize)
  {
    rect buttonRect = rect(vec2(screenSize.x - margin, 6.0), vec2(margin - 12));
    buttonRect.origin.x -= buttonSpacing;
    
    for (Button* buttonPointer : buttons)
    {
      buttonPointer->SetArea(buttonRect);
      buttonRect.origin.x -= margin;
    }
  }

  void WindowManager::Draw() 
  {
    for (Button* buttonPointer : buttons)
      buttonPointer->Draw();
  }
}