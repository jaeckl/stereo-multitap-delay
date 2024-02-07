#include "LowpassButton.h"
#include <BinaryData.h>
LowpassButton::LowpassButton(const juce::String &name)
    : juce::DrawableButton(name, juce::DrawableButton::ButtonStyle::ImageRaw) {
  auto instream1 = juce::MemoryInputStream(
      BinaryData::lowpass_button_png,
      BinaryData::lowpass_button_pngSize,
      false);
  auto instream2 = juce::MemoryInputStream(
      BinaryData::lowpass_button_highlight_png,
      BinaryData::lowpass_button_highlight_pngSize,
      false);
  auto instream3 = juce::MemoryInputStream(
      BinaryData::lowpass_button_pressed_png,
      BinaryData::lowpass_button_pressed_pngSize,
      false);
  auto instream4 = juce::MemoryInputStream(
      BinaryData::lowpass_button_pressed_highlight_png,
      BinaryData::lowpass_button_pressed_highlight_pngSize,
      false);
  normalImage = juce::Drawable::createFromImageDataStream(instream1);
  overImage = juce::Drawable::createFromImageDataStream(instream2);
  normalImageOn = juce::Drawable::createFromImageDataStream(instream3);
  overImageOn = juce::Drawable::createFromImageDataStream(instream4);
  setImages(
      normalImage.get(),
      overImage.get(),
      nullptr,
      nullptr,
      normalImageOn.get(),
      overImageOn.get(),
      nullptr,
      nullptr);
  setToggleable(true);
  setClickingTogglesState(true);
  setToggleState(false, juce::NotificationType::dontSendNotification);
  setColour(
      juce::DrawableButton::backgroundOnColourId,
      juce::Colours::transparentWhite);
}
