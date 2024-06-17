#include "button.cpp"

namespace ui {
#ifndef DRAG_BUTTON
#define DRAG_BUTTON
  class DragButton : public Button
  {
    private:
      vec2 dragStartPosition;
      vec2 lastDragPosition;
      vec2 mouseNegation;

    public:
      BoundVec2Callback onDrag;
      BoundVec2Callback onDragFinished;
      KeyboardKey holdKey = KEY_NULL;

      DragButton(vec2);
      DragButton(rect, vec2);
      DragButton(vec2, vec2, vec2);

      void Update(vec2) override;
      void Press() override;
      void Release() override;
      void ReleaseWithoutHover() override;
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
      Press();
    if (isPressed && mousePosition != lastDragPosition)
    {
      vec2 offset = mousePosition - lastDragPosition;
      if(onDrag) onDrag(offset);
      lastDragPosition = mousePosition;
      lastDragPosition += offset * mouseNegation;
    }
  }

  void DragButton::Press()
  {
    Button::Press();
    dragStartPosition = GetMousePosition();
    lastDragPosition = dragStartPosition;
  }

  void DragButton::Release()
  {
    Button::Release();
    vec2 mousePosition = GetMousePosition();
    if (onDragFinished) onDragFinished(mousePosition - dragStartPosition);
  }

  void DragButton::ReleaseWithoutHover()
  {
    Button::ReleaseWithoutHover();
    vec2 mousePosition = GetMousePosition();
    if (onDragFinished) onDragFinished(mousePosition - dragStartPosition);
  }

  void DragButton::Draw()
  {
    Color colour = isHovered? hoveredColour: normalColour;
    if (isPressed) colour = hoveredColour;
    DrawWithColour(colour);
  }
#endif
}