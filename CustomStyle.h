#pragma once
#include <BinaryData.h>
#include <functional>
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_gui_basics/juce_gui_basics.h>

class CustomButtonStyle : public juce::LookAndFeel_V4 {
public:
  explicit CustomButtonStyle(const juce::Colour &colour) {
    setColour(juce::TextButton::buttonColourId, colour.darker(1.0));
    setColour(juce::TextButton::buttonOnColourId, colour);
  }
  void drawButtonBackground(
      juce::Graphics &g,
      juce::Button &button,
      const juce::Colour &backgroundColour,
      bool shoulDrawButtonAsHighlighted,
      bool shouldDrawButtonAsDown) override {
    juce::ignoreUnused(shoulDrawButtonAsHighlighted);
    g.setColour(backgroundColour);
    if (shouldDrawButtonAsDown)
      g.setColour(backgroundColour.withLightness(0.8));
    g.fillRect(button.getLocalBounds());
  }
};

class CustomRotarySliderStyle : public juce::LookAndFeel_V4 {
private:
  juce::Image rotaryKnobImage;
  juce::Colour baseColour;
  juce::Font textFont;
  juce::Font rotaryValueFont;
  juce::String rotaryUnitText;
  std::function<juce::String(juce::Slider *)> sliderFormatingFunction;

public:
  explicit CustomRotarySliderStyle(const juce::Colour &colour)
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

  void setRotaryUnitText(juce::String text) { rotaryUnitText = text; }
  void setSliderFormater(std::function<juce::String(juce::Slider *)> formater) {
    sliderFormatingFunction = formater;
  }
  void drawRotarySlider(
      juce::Graphics &g,
      int x,
      int y,
      int width,
      int height,
      float sliderPos,
      const float rotaryStartAngle,
      const float rotaryEndAngle,
      juce::Slider &slider) override {
    juce::ignoreUnused(width);
    juce::ignoreUnused(height);
    auto angle =
        rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    float centerX = rotaryKnobImage.getWidth() / 2.0;
    float centerY = rotaryKnobImage.getHeight() / 2.0;
    auto transform = juce::AffineTransform()
                         .rotated(angle, centerX, centerY)
                         .translated(x, y);
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
  }
};
