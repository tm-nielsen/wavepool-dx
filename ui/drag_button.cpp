#include "button.cpp"

namespace ui {
  using namespace utils;

  class DragButton : public Button
  {
    private:
      vec2 dragStartPosition;
      vec2 lastDragPosition;

      vec2 mouseNegation;

    public:
      std::function<void(vec2)> onDrag = [](vec2 v){};
      std::function<void(vec2)> onDragFinished = [](vec2){};
      KeyboardKey holdKey = KEY_NULL;

      DragButton(vec2);
      DragButton(rect, vec2);
      DragButton(vec2, vec2, vec2);

      void Update(vec2) override;
      void OnPressed() override;
      void OnReleased() override;
      void OnReleasedWithoutHover() override;
      void Draw() override;
  };

  DragButton::DragButton(vec2 mouseNegation = vec2()):
    Button(), mouseNegation{mouseNegation} {};
  DragButton::DragButton(rect area, vec2 mouseNegation = vec2()):
    Button(area), mouseNegation{mouseNegation} {};
  DragButton::DragButton(vec2 origin, vec2 size, vec2 mouseNegation = vec2()):
    Button(origin, size), mouseNegation{mouseNegation} {};


  void DragButton::Update(vec2 mousePosition)
  {
    Button::Update(mousePosition);
    if (IsKeyDown(holdKey) && !isPressed && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
      OnPressed();
    if (isPressed && mousePosition != lastDragPosition)
    {
      vec2 offset = mousePosition - lastDragPosition;
      onDrag(offset);
      lastDragPosition = mousePosition;
      lastDragPosition += offset * mouseNegation;
    }
  }

  void DragButton::OnPressed()
  {
    Button::OnPressed();
    dragStartPosition = GetMousePosition();
    lastDragPosition = dragStartPosition;
  }

  void DragButton::OnReleased()
  {
    Button::OnReleased();
    vec2 mousePosition = GetMousePosition();
    onDragFinished(mousePosition - dragStartPosition);
  }

  void DragButton::OnReleasedWithoutHover()
  {
    Button::OnReleasedWithoutHover();
    vec2 mousePosition = GetMousePosition();
    onDragFinished(mousePosition - dragStartPosition);
  }

  void DragButton::Draw()
  {
    Color colour = isHovered? hoveredColour: normalColour;
    if (isPressed) colour = hoveredColour;
    DrawWithColour(colour);
  }
}