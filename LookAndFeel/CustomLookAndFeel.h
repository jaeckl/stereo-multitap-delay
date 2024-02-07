#pragma once
#include <algorithm>
#include <juce_gui_basics/juce_gui_basics.h>
#include <memory>
class CustomLookAndFeel : public juce::LookAndFeel_V4 {
public:
  explicit CustomLookAndFeel(bool flat = false);
  void drawButtonBackground(
      juce::Graphics &g,
      juce::Button &button,
      const juce::Colour &backgroundColour,
      bool shoulDrawButtonAsHighlighted,
      bool shouldDrawButtonAsDown) override;

  juce::Font getLabelFont(juce::Label &) override;
  juce::Font getTextButtonFont(juce::TextButton &button, int height) override;
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

private:
  bool flat = false;
  juce::Font font;
  juce::Image iconFolderSmall;
  juce::Image iconFolderSmallOn;
  juce::Image iconFileSmall;
  juce::Image iconFileSmallOn;
  juce::Image loadImage(const char *resourceName, int size);
};
