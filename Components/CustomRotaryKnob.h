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

#pragma once
#include "LookAndFeel/CustomRotarySliderStyle.h"
#include <juce_gui_basics/juce_gui_basics.h>
class CustomRotaryKnob : public juce::Slider {
private:
  CustomRotarySliderStyle style;

public:
  explicit CustomRotaryKnob(const juce::String &name, juce::Colour colour);
  void setTitle(const juce::String &name);
  void setUnitName(const juce::String &name);
  void setValueFormater(std::function<juce::String(juce::Slider *)> formater);
};
