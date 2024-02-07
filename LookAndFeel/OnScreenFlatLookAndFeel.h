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
  const juce::Image *getDefaultDirectoryImage();
  const juce::Image *getDefaultDirectoryImageOn();
  const juce::Image *getDefaultFileImage();
  const juce::Image *getDefaultFileImageOn();

private:
  juce::Image iconFolderSmall;
  juce::Image iconFolderSmallOn;
  juce::Image iconFileSmall;
  juce::Image iconFileSmallOn;
  juce::Image loadImage(const char *resourceName, int size);
};
