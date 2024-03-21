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

#include "BypassButton.h"
#include <BinaryData.h>
BypassButton::BypassButton(const juce::String &name)
    : juce::DrawableButton(name, juce::DrawableButton::ButtonStyle::ImageRaw) {
  auto instream1 = juce::MemoryInputStream(
      BinaryData::bypass_button_png, BinaryData::bypass_button_pngSize, false);
  auto instream2 = juce::MemoryInputStream(
      BinaryData::bypass_button_highlight_png,
      BinaryData::bypass_button_highlight_pngSize,
      false);
  auto instream3 = juce::MemoryInputStream(
      BinaryData::bypass_button_pressed_png,
      BinaryData::bypass_button_pressed_pngSize,
      false);
  auto instream4 = juce::MemoryInputStream(
      BinaryData::bypass_button_pressed_highlight_png,
      BinaryData::bypass_button_pressed_highlight_pngSize,
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
}
