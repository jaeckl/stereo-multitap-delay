#pragma once
#include "CustomStyle.h"
#include <juce_gui_basics/juce_gui_basics.h>
class CustomRotaryKnob : public juce::Slider {
private:
  CustomRotarySliderStyle style;

public:
  explicit CustomRotaryKnob(const juce::String &name, juce::Colour colour)
      : style(colour) {
    setName(name);
    setSliderStyle(juce::Slider::RotaryVerticalDrag);
    setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    setLookAndFeel(&style);
  }
  void setUnitName(const juce::String &name) { style.setRotaryUnitText(name); }
  void setValueFormater(std::function<juce::String(juce::Slider *)> formater) {
    style.setSliderFormater(formater);
  }
};
