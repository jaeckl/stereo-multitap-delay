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
