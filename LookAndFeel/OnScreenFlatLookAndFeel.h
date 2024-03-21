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

#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

class OnScreenFlatLookAndFeel : public juce::LookAndFeel_V4 {
public:
  OnScreenFlatLookAndFeel();
  void drawButtonBackground(
      juce::Graphics &g,
      juce::Button &button,
      const juce::Colour &backgroundColour,
      bool shoulDrawButtonAsHighlighted,
      bool shouldDrawButtonAsDown) override;
  void drawFileBrowserRow(
      juce::Graphics &,
      int width,
      int height,
      const juce::File &file,
      const juce::String &filename,
      juce::Image *icon,
      const juce::String &fileSizeDescription,
      const juce::String &fileTimeDescription,
      bool isDirectory,
      bool isItemSelected,
      int itemIndex,
      juce::DirectoryContentsDisplayComponent &) override;
  void drawScrollbar(
      juce::Graphics &g,
      juce::ScrollBar &scrollbar,
      int x,
      int y,
      int width,
      int height,
      bool isScrollbarVertical,
      int thumbStartPosition,
      int thumbSize,
      bool isMouseOver,
      bool isMouseDown) override;
  void drawComboBox(
      juce::Graphics &,
      int width,
      int height,
      bool isButtonDown,
      int buttonX,
      int buttonY,
      int buttonW,
      int buttonH,
      juce::ComboBox &) override;
  void
  drawPopupMenuBackground(juce::Graphics &, int width, int height) override;
  const juce::Image *getDefaultDirectoryImage();
  const juce::Image *getDefaultDirectoryImageOn();
  const juce::Image *getDefaultFileImage();
  const juce::Image *getDefaultFileImageOn();

private:
  juce::Image iconFolderSmall;
  juce::Image iconFolderSmallOn;
  juce::Image iconFileSmall;
  juce::Image iconFileSmallOn;

  juce::Image iconDropDown;
  juce::Image loadImage(const char *resourceName, size_t size);
};
