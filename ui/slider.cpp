#include "raylib.h"
#include "ui_callback_types.cpp"
#include "drag_button.cpp"

namespace ui {
  class Slider
  {
    private:
      DragButton handle;
      rect dragArea;
      rect backgroundRect;

      Color normalColour;
      Color hoveredColour;
      float borderThickness;

    public:
      BoundFloatCallback onValueChanged;
      BoundFloatCallback onHandleReleased;
      float value = 0;

      Slider();
      Slider(rect, vec2);
      void BindHandleCallbacks();

      void SetStyle(Color, Color, float);
      void SetShape(rect, vec2);
      void Update(vec2);
      void OnHandleMoved(vec2);
      void OnHandleMovementFinished(vec2);
      void SetValue(float);
      void Draw();
  };

  Slider::Slider()
  {
    handle = DragButton();
    dragArea = rect();
    backgroundRect = rect();
  }

  Slider::Slider(rect area, vec2 handleSize)
  {
    Slider();
    SetShape(area, handleSize);
  }

  void Slider::BindHandleCallbacks()
  {
    handle.onDrag = std::bind(OnHandleMoved, this, _1);
    handle.onDragFinished = std::bind(OnHandleMovementFinished, this, _1);
  }

  void Slider::SetStyle(Color normal, Color hovered, float thickness)
  {
    normalColour = normal;
    hoveredColour = hovered;
    borderThickness = thickness;
    handle.SetStyle(normal, hovered, thickness);
  }

  void Slider::SetShape(rect area, vec2 handleSize)
  {
    handle.SetArea(rect(vec2(), handleSize));
    
    dragArea = area;
    dragArea.origin.y = area.GetCentre().y;
    dragArea.size.y = 0;
    SetValue(value);

    backgroundRect = area;
  }

  void Slider::Update(vec2 mousePosition)
  {
    handle.Update(mousePosition);
  }

  void Slider::OnHandleMoved(vec2 offset)
  {
    vec2 handlePosition = handle.GetCentrePosition();
    handlePosition = dragArea.ClampPoint(handlePosition + offset);
    handle.SetCentrePosition(handlePosition);


    float dragDistance = handlePosition.x - dragArea.origin.x;
    value = dragDistance / dragArea.size.x;
    if (onValueChanged) onValueChanged(value);
  }

  void Slider::OnHandleMovementFinished(vec2 offset)
  {
    if (onHandleReleased) onHandleReleased(value);
  }

  void Slider::SetValue(float newValue)
  {
    value = newValue;
    float dragDistance = newValue * dragArea.size.x;
    handle.SetCentrePosition(dragArea.origin + RIGHT * dragDistance);
  }

  void Slider::Draw()
  {
    backgroundRect.DrawRounded(borderThickness, normalColour);
    handle.Draw();
  }
}