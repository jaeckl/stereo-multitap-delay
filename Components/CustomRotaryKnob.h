#pragma once
#include "LookAndFeel/CustomRotarySliderStyle.h"
#include <juce_gui_basics/juce_gui_basics.h>
class CustomRotaryKnob : public juce::Slider {
private:
  CustomRotarySliderStyle style;

public:
  explicit CustomRotaryKnob(const juce::String &name, juce::Colour colour);
  void setUnitName(const juce::String &name);
  void setValueFormater(std::function<juce::String(juce::Slider *)> formater);
};
