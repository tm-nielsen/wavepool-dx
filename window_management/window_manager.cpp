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

      float buttonSpacing;

    public:
      BoundCallback onCloseButtonPressed;
      BoundCallback onSettingsButtonPressed;
      BoundVec2Callback onWindowResized;

      WindowManager(vec2);
      void LoadResources();
      void UnloadResources();
      void SetStyle(Color, Color, float);
      void Update();
      void OnWindowResized(vec2, float);
      void Draw();

      void CloseProgram();
      void ToggleSettingsMenu();
      void MoveWindow(vec2);
      void ResizeWindow(vec2, vec2);
  };

  WindowManager::WindowManager(vec2 minimumSize)
  {
    closeProgramButton = Button();
    closeProgramButton.onRelease = std::bind(CloseProgram, this);

    toggleSettingsMenuButton = Button();
    toggleSettingsMenuButton.onPress = std::bind(ToggleSettingsMenu, this);

    moveButton = DragButton(vec2(-1, -1));
    moveButton.onDrag = std::bind(MoveWindow, this, _1);
    moveButton.holdKey = KEY_SPACE;

    resizeButton = DragButton(UP);
    resizeButton.onDrag = std::bind(ResizeWindow, this, _1, minimumSize);
    resizeButton.holdKey = KEY_R;

    buttons = {&closeProgramButton, &toggleSettingsMenuButton, &moveButton, &resizeButton};
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
    buttonSpacing = thickness * 2;
    for (Button* buttonPointer : buttons)
      buttonPointer->SetStyle(normalColour, hoverColour, thickness);
  }

  void WindowManager::Update()
  {
    vec2 mousePosition = GetMousePosition();
    for (Button* buttonPointer : buttons)
      buttonPointer->Update(mousePosition);
  }

  void WindowManager::OnWindowResized(vec2 screenSize, float margin)
  {
    rect buttonRect = rect(vec2(screenSize.x - margin, buttonSpacing / 2), vec2(margin - buttonSpacing));
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
    if (onCloseButtonPressed) onCloseButtonPressed();
  }

  void WindowManager::ToggleSettingsMenu()
  {
    if (onSettingsButtonPressed) onSettingsButtonPressed();
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

    if (onWindowResized) onWindowResized(windowSize);
  }
}