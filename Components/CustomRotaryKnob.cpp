/*
Copyright (C) 2024  Ludwig Jäck

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
*/

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
void CustomRotaryKnob::setTitle(const juce::String &name) {
  style.setTitle(name);
}
