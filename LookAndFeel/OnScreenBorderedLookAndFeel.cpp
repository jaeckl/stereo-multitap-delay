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

#include "OnScreenBorderedLookAndFeel.h"

void OnScreenBorderedLookAndFeel::drawButtonBackground(
    juce::Graphics &g,
    juce::Button &button,
    const juce::Colour &backgroundColour,
    bool shoulDrawButtonAsHighlighted,
    bool shouldDrawButtonAsDown) {
  OnScreenFlatLookAndFeel::drawButtonBackground(
      g,
      button,
      backgroundColour,
      shoulDrawButtonAsHighlighted,
      shouldDrawButtonAsDown);

  if (button.isToggleable() && !button.getToggleState()) {
    g.setColour(button.findColour(juce::TextButton::buttonColourId)
                    .withMultipliedAlpha(2.0));
    g.drawRect(button.getLocalBounds(), 2);
  }
}
