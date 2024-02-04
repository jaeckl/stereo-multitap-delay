#include "CustomButtonStyle.h"
CustomButtonStyle::CustomButtonStyle(const juce::Colour &colour) {
  setColour(juce::TextButton::buttonColourId, colour.darker(1.0));
  setColour(juce::TextButton::buttonOnColourId, colour);
  setColour(juce::TextButton::textColourOnId, juce::Colours::black);
  setColour(juce::TextButton::textColourOffId, juce::Colours::white);
}
void CustomButtonStyle::drawButtonBackground(
    juce::Graphics &g,
    juce::Button &button,
    const juce::Colour &backgroundColour,
    bool shoulDrawButtonAsHighlighted,
    bool shouldDrawButtonAsDown) {
  juce::ignoreUnused(shoulDrawButtonAsHighlighted);
  g.setColour(backgroundColour.withAlpha(0.2f).withLightness(0.6));
  if (!button.getToggleState()) {
    g.setColour(backgroundColour.withAlpha(0.2f));
  }
  if (shouldDrawButtonAsDown) {
    g.setColour(backgroundColour.withAlpha(0.2f).withLightness(0.6));
  }
  g.fillAll();
}
