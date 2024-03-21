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
#include <BinaryData.h>
#include <functional>
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_gui_basics/juce_gui_basics.h>

class CustomRotarySliderStyle : public juce::LookAndFeel_V4 {
private:
  juce::Image rotaryKnobImage;
  juce::Image rotaryKnobBackground;
  juce::Colour baseColour;
  juce::Font textFont;
  juce::Font rotaryValueFont;
  juce::String rotaryUnitText;
  juce::String rotaryTitle;
  std::function<juce::String(juce::Slider *)> sliderFormatingFunction;

public:
  explicit CustomRotarySliderStyle(const juce::Colour &colour);

  void setRotaryUnitText(const juce::String &text);
  void setTitle(const juce::String &text);
  void setSliderFormater(std::function<juce::String(juce::Slider *)> formater);
  void drawRotarySlider(
      juce::Graphics &g,
      int x,
      int y,
      int width,
      int height,
      float sliderPos,
      const float rotaryStartAngle,
      const float rotaryEndAngle,
      juce::Slider &slider) override;
};
