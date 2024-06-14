#include "raylib.h"
#include "../ui/button.cpp"

namespace settings {
  using namespace ui;

  class SettingsMenu
  {
    private:

    public:
      SettingsMenu(vec2, float, float);
      void SetStyle(Color, Color, float);
      void Update();
      void Draw();
  };

  SettingsMenu::SettingsMenu(vec2 screenSize, float margin, float buttonSpacing = 12)
  {
    
  }

  void SettingsMenu::SetStyle(Color normalColour, Color hoverColour, float thickness)
  {
    
  }

  void SettingsMenu::Update()
  {
    
  }

  void SettingsMenu::Draw() 
  {
    
  }
}