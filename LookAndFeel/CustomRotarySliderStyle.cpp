#include "CustomRotarySliderStyle.h"
#include <BinaryData.h>
#include <functional>
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_gui_basics/juce_gui_basics.h>

CustomRotarySliderStyle::CustomRotarySliderStyle(const juce::Colour &colour)
    : baseColour(colour) {
  auto inputStream = juce::MemoryInputStream(
      BinaryData::rotary_knob_png, BinaryData::rotary_knob_pngSize, false);
  auto format = juce::ImageFileFormat::findImageFormatForStream(inputStream);
  rotaryKnobImage = format->decodeImage(inputStream);

  rotaryValueFont.setHeight(16);
  rotaryValueFont.setBold(true);
  textFont.setHeight(12);

  sliderFormatingFunction = [](juce::Slider *slider) -> juce::String {
    return juce::String(slider->getValue());
  };
}

void CustomRotarySliderStyle::setRotaryUnitText(const juce::String &text) {
  rotaryUnitText = text;
}
void CustomRotarySliderStyle::setTitle(const juce::String &text) {
  rotaryTitle = text;
}
void CustomRotarySliderStyle::setSliderFormater(
    std::function<juce::String(juce::Slider *)> formater) {
  sliderFormatingFunction = formater;
}
void CustomRotarySliderStyle::drawRotarySlider(
    juce::Graphics &g,
    int x,
    int y,
    int width,
    int height,
    float sliderPos,
    const float rotaryStartAngle,
    const float rotaryEndAngle,
    juce::Slider &slider) {
  juce::ignoreUnused(width);
  juce::ignoreUnused(height);
  auto angle =
      rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
  float centerX = rotaryKnobImage.getWidth() / 2.0;
  float centerY = rotaryKnobImage.getHeight() / 2.0;
  auto transform =
      juce::AffineTransform().rotated(angle, centerX, centerY).translated(x, y);
  g.drawImageTransformed(rotaryKnobImage, transform);

  g.setColour(baseColour);

  g.setFont(rotaryValueFont);
  auto format_string = sliderFormatingFunction(&slider);
  g.drawFittedText(
      format_string,
      slider.getLocalBounds(),
      juce::Justification::centred,
      true);
  g.setColour(juce::Colours::grey);
  g.setFont(textFont);
  g.drawFittedText(
      rotaryUnitText,
      slider.getLocalBounds().reduced(20, 24),
      juce::Justification::centredBottom,
      true);
  g.drawFittedText(
      rotaryTitle,
      slider.getLocalBounds().reduced(20, 24),
      juce::Justification::centredTop,
      true);
}
