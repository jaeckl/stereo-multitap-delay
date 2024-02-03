#include "CustomRotaryKnob.h"

CustomRotaryKnob::CustomRotaryKnob(
    const juce::String &name,
    juce::Colour colour)
    : style(colour) {
  setName(name);
  setSliderStyle(juce::Slider::RotaryVerticalDrag);
  setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
  setLookAndFeel(&style);
}
void CustomRotaryKnob::setUnitName(const juce::String &name) {
  style.setRotaryUnitText(name);
}
void CustomRotaryKnob::setValueFormater(
    std::function<juce::String(juce::Slider *)> formater) {
  style.setSliderFormater(formater);
}
