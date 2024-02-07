#pragma once
#include "LookAndFeel/OnScreenFlatLookAndFeel.h"
#include <juce_gui_basics/juce_gui_basics.h>

class OnScreenBorderedLookAndFeel : public OnScreenFlatLookAndFeel {
public:
  void drawButtonBackground(
      juce::Graphics &g,
      juce::Button &button,
      const juce::Colour &backgroundColour,
      bool shoulDrawButtonAsHighlighted,
      bool shouldDrawButtonAsDown) override;
};
