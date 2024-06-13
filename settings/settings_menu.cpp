#include "raylib.h"
#include <functional>
#include "../ui/button.cpp"
#include <iostream>

namespace settings {
  using namespace ui;

  class SettingsMenu
  {
    private:
      Button openButton;

      void OnOpenButtonClicked();

    public:
      bool isOpen = false;

      SettingsMenu(vec2, float);
      void SetStyle(Color, Color, float);
      void Update();
      void Draw();
  };

  SettingsMenu::SettingsMenu(vec2 screenSize, float margin) {
    auto onOpenButtonClicked = [this]() {
      std::cout << "open button clicked\n";
      isOpen = !isOpen;
    };
    openButton = Button(vec2(screenSize.x - margin, 0.0), vec2(margin), onOpenButtonClicked);
  }

  void SettingsMenu::SetStyle(Color normalColour, Color hoverColour, float thickness) {
    openButton.SetStyle(normalColour, hoverColour, thickness);
  }

  void SettingsMenu::Update() {
    vec2 mousePosition = GetMousePosition();
    openButton.Update(mousePosition);
  }

  void SettingsMenu::Draw() {
    openButton.Draw();
  }


  void SettingsMenu::OnOpenButtonClicked() {
    std::cout << "open button clicked\n";
    isOpen = !isOpen;
  }
}