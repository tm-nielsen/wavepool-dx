#include "button.cpp"

namespace ui {
  using namespace utils;

  class DragButton : public Button
  {
    private:
      vec2 dragStartPosition;
      vec2 lastDragPosition;

      bool negateMouseMovement = false;

    public:
      std::function<void(vec2)> onDrag = [](vec2 v){};
      std::function<void(vec2)> onDragFinished = [](vec2){};

      DragButton();
      DragButton(rect, bool);
      DragButton(vec2, vec2, bool);

      void Update(vec2) override;
      void OnPressed() override;
      void OnReleased() override;
      void OnReleasedWithoutHover() override;
      void Draw() override;
  };

  DragButton::DragButton(): Button() {};
  DragButton::DragButton(rect area, bool affectsMousePosition = false):
    Button(area), negateMouseMovement{affectsMousePosition} {};
  DragButton::DragButton(vec2 origin, vec2 size, bool affectsMousePosition = false):
    Button(origin, size), negateMouseMovement{affectsMousePosition} {};


  void DragButton::Update(vec2 mousePosition)
  {
    Button::Update(mousePosition);
    if (isPressed && mousePosition != lastDragPosition)
    {
      vec2 offset = mousePosition - lastDragPosition;
      onDrag(offset);
      lastDragPosition = mousePosition;
      if (negateMouseMovement)
        lastDragPosition -= offset;
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