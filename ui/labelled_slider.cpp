#include "composite_ui_element.cpp"
#include "label.cpp"
#include "slider.cpp"

namespace ui {
  class LabelledSlider: public CompositeUIElement
  {
    private:
      Slider slider;
      Label label;

    public:
      BoundFloatCallback onValueChanged;
      BoundFloatCallback onHandleReleased;
      float value = 0;

      LabelledSlider(const char*);
      void BindCallbacks();

      void SetStyle(Color, Color, float);
      void SetArea(rect);
      void SetArea(rect, float);
      void Update(vec2);
      void OnSliderValueChanged(float);
      void OnSliderHandleReleased();
      void SetValue(float);
      void Draw();
  };

  LabelledSlider::LabelledSlider(const char* labelText = "slider")
  {
    slider = Slider();
    label = Label(labelText);
  }

  void LabelledSlider::BindCallbacks()
  {
    slider.BindCallbacks();
    slider.onValueChanged = std::bind(OnSliderValueChanged, this, _1);
    slider.onHandleReleased = std::bind(OnSliderHandleReleased, this);
  }

  void LabelledSlider::SetStyle(Color normal, Color hovered, float thickness)
  {
    slider.SetStyle(normal, hovered, thickness);
    label.SetStyle(normal, thickness / 4);
  }

  void LabelledSlider::SetArea(rect area)
  {
    SetArea(area, 0.5);
  }
  void LabelledSlider::SetArea(rect area, float labelShare)
  {
    rect labelArea = area;
    labelArea.size.x *= labelShare;
    label.SetArea(labelArea);

    rect sliderArea = area;
    sliderArea.size.x *= (1 - labelShare);
    sliderArea.origin.x = area.origin.x + area.size.x - sliderArea.size.x;
    sliderArea.size.y /= 3;
    sliderArea.origin.y += area.size.y / 3;
    vec2 handleSize = vec2(area.size.y);
    handleSize.x /= 2;
    slider.SetShape(sliderArea, handleSize);
  }

  void LabelledSlider::Update(vec2 mousePosition)
  {
    slider.Update(mousePosition);
  }

  void LabelledSlider::OnSliderValueChanged(float sliderValue)
  {
    value = sliderValue;
    if (onValueChanged) onValueChanged(value);
  }

  void LabelledSlider::OnSliderHandleReleased()
  {
    if (onHandleReleased) onHandleReleased(value);
  }

  void LabelledSlider::SetValue(float newValue)
  {
    value = newValue;
    slider.SetValue(newValue);
  }

  void LabelledSlider::Draw()
  {
    label.Draw();
    slider.Draw();
  }
}