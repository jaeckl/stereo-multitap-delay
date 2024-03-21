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

#include "FileSystemButton.h"
#include <BinaryData.h>
FileSystemButton::FileSystemButton(const juce::String &name)
    : juce::DrawableButton(
          name,
          juce::DrawableButton::ButtonStyle::ImageFitted) {
  auto instream1 = juce::MemoryInputStream(
      BinaryData::icon_folder_png, BinaryData::icon_folder_pngSize, false);
  auto instream2 = juce::MemoryInputStream(
      BinaryData::icon_folder_on_png,
      BinaryData::icon_folder_on_pngSize,
      false);
  normalImage = juce::Drawable::createFromImageDataStream(instream1);
  normalImageOn = juce::Drawable::createFromImageDataStream(instream2);
  setImages(
      normalImage.get(),
      nullptr,
      nullptr,
      nullptr,
      normalImageOn.get(),
      nullptr,
      nullptr,
      nullptr);
  setToggleable(true);
  setClickingTogglesState(true);
  setToggleState(false, juce::NotificationType::dontSendNotification);
  setColour(
      juce::DrawableButton::backgroundColourId,
      juce::Colours::transparentWhite);
  setColour(
      juce::DrawableButton::backgroundOnColourId,
      juce::Colours::transparentWhite);
}
