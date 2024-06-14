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
      Button toggleSettingsMenuButton;
      DragButton moveButton;
      DragButton resizeButton;

      std::vector<Button*> buttons;

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

      void CloseProgram();
      void ToggleSettingsMenu();
      void MoveWindow(vec2);
      void ResizeWindow(vec2, vec2);
  };

  WindowManager::WindowManager(vec2 size, vec2 minimumSize, float margin, float buttonSpacing = 12):
    margin{margin}, buttonSpacing{buttonSpacing}
  {
    closeProgramButton = Button();
    closeProgramButton.onRelease = [this](){CloseProgram();};

    toggleSettingsMenuButton = Button();
    toggleSettingsMenuButton.onPress = [this](){ToggleSettingsMenu();};

    moveButton = DragButton(vec2(-1, -1));
    moveButton.onDrag = [this](vec2 offset){MoveWindow(offset);};
    moveButton.holdKey = KEY_SPACE;

    resizeButton = DragButton(UP);
    resizeButton.onDrag = [this, minimumSize](vec2 offset){ResizeWindow(offset, minimumSize);};
    resizeButton.holdKey = KEY_R;

    buttons = {&closeProgramButton, &toggleSettingsMenuButton, &moveButton, &resizeButton};
    OnWindowResized(size);
  }

  void WindowManager::LoadResources()
  {
    closeProgramButton.LoadResources("resources/icons/close_icon.png");
    toggleSettingsMenuButton.LoadResources("resources/icons/settings_icon.png");
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


  void WindowManager::CloseProgram()
  {
    shouldExitProgram = true;
  }

  void WindowManager::ToggleSettingsMenu()
  {
    settingsMenuIsOpen = !settingsMenuIsOpen;
  }

  void WindowManager::MoveWindow(vec2 offset)
  {
    vec2 windowPosition = GetWindowPosition();
    windowPosition += offset;
    SetWindowPosition(windowPosition.x, windowPosition.y);
  }

  void WindowManager::ResizeWindow(vec2 offset, vec2 minimumSize)
  {
    vec2 windowSize = vec2(GetScreenWidth(), GetScreenHeight());
    float currentWindowHeight = windowSize.y;
    
    offset.y *= -1;
    windowSize += offset;
    windowSize = windowSize.ClampLower(minimumSize);

    vec2 windowPosition = GetWindowPosition();
    windowPosition.y -= windowSize.y - currentWindowHeight;

    SetWindowSize(windowSize.x, windowSize.y);
    SetWindowPosition(windowPosition.x, windowPosition.y);

    windowResized = true;
    OnWindowResized(windowSize);
  }
}