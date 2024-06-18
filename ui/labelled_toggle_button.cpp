#include "composite_ui_element.cpp"
#include "label.cpp"
#include "toggle_button.cpp"

namespace ui {
  class LabelledToggleButton: public CompositeUIElement
  {
    private:
      ToggleButton toggleButton;
      Label label;

    public:
      BoundBooleanCallback onToggle;
      bool isToggled = false;

      LabelledToggleButton(const char*);

      void LoadResources();
      void UnloadResources();
      void BindCallbacks();

      void SetStyle(Color, Color, float);
      void SetArea(rect);
      void SetArea(rect, float);
      void Update(vec2);
      void OnButtonToggled(bool);
      void Draw();
  };

  LabelledToggleButton::LabelledToggleButton(const char* labelText = "toggle")
  {
    toggleButton = ToggleButton();
    label = Label(labelText);
  }

  void LabelledToggleButton::LoadResources()
  {
    toggleButton.LoadResources("resources/icons/close_icon.png");
  }

  void LabelledToggleButton::UnloadResources()
  {
    toggleButton.UnloadResources();
  }

  void LabelledToggleButton::BindCallbacks()
  {
    toggleButton.onToggle = std::bind(OnButtonToggled, this, _1);
  }

  void LabelledToggleButton::SetStyle(Color normal, Color hovered, float thickness)
  {
    toggleButton.SetStyle(normal, hovered, thickness, 15);
    label.SetStyle(normal, thickness / 4);
  }

  void LabelledToggleButton::SetArea(rect area)
  {
    SetArea(area, 0.5);
  }
  void LabelledToggleButton::SetArea(rect area, float labelShare)
  {
    rect buttonArea = area;
    buttonArea.size.x = area.size.y;
    buttonArea.origin.x += area.size.x - area.size.y;
    toggleButton.SetArea(buttonArea);

    rect labelArea = area;
    labelArea.size.x -= buttonArea.size.x;
    label.SetArea(labelArea);
  }

  void LabelledToggleButton::Update(vec2 mousePosition)
  {
    toggleButton.Update(mousePosition);
  }

  void LabelledToggleButton::OnButtonToggled(bool buttonIsToggled)
  {
    isToggled = buttonIsToggled;
    if (onToggle) onToggle(isToggled);
  }

  void LabelledToggleButton::Draw()
  {
    label.Draw();
    toggleButton.Draw();
  }
}