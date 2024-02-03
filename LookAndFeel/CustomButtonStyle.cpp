#include "CustomButtonStyle.h"
CustomButtonStyle::CustomButtonStyle(const juce::Colour &colour) {
  setColour(juce::TextButton::buttonColourId, colour.darker(1.0));
  setColour(juce::TextButton::buttonOnColourId, colour);
}
void CustomButtonStyle::drawButtonBackground(
    juce::Graphics &g,
    juce::Button &button,
    const juce::Colour &backgroundColour,
    bool shoulDrawButtonAsHighlighted,
    bool shouldDrawButtonAsDown) {
  juce::ignoreUnused(shoulDrawButtonAsHighlighted);
  g.setColour(backgroundColour);
  if (shouldDrawButtonAsDown)
    g.setColour(backgroundColour.withLightness(0.8));
  g.fillRect(button.getLocalBounds());
}
