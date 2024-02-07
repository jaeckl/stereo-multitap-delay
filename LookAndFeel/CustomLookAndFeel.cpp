#include "CustomLookAndFeel.h"
#include "Constants.h"
#include <BinaryData.h>
CustomLookAndFeel::CustomLookAndFeel(bool isFlat)
    : flat(isFlat) {
  iconFolderSmall = loadImage(
      BinaryData::icon_folder_small_png, BinaryData::icon_folder_small_pngSize);
  iconFolderSmallOn = loadImage(
      BinaryData::icon_folder_small_on_png,
      BinaryData::icon_folder_small_on_pngSize);
  iconFileSmall = loadImage(
      BinaryData::icon_file_small_png, BinaryData::icon_file_small_pngSize);
  iconFileSmallOn = loadImage(
      BinaryData::icon_file_small_on_png,
      BinaryData::icon_file_small_on_pngSize);
}
juce::Image
CustomLookAndFeel::loadImage(const char *resourceName, int resourceSize) {
  auto instream = juce::MemoryInputStream(resourceName, resourceSize, false);
  auto format = juce::ImageFileFormat::findImageFormatForStream(instream);
  return format->decodeImage(instream);
}

void CustomLookAndFeel::drawButtonBackground(
    juce::Graphics &g,
    juce::Button &button,
    const juce::Colour &backgroundColour,
    bool shoulDrawButtonAsHighlighted,
    bool shouldDrawButtonAsDown) {
  juce::ignoreUnused(shoulDrawButtonAsHighlighted);

  if (button.isToggleable()) {
    if (button.getToggleState())
      g.setColour(button.findColour(juce::TextButton::buttonOnColourId));
    else
      g.setColour(button.findColour(juce::TextButton::buttonColourId));
  } else {
    if (shouldDrawButtonAsDown)
      g.setColour(button.findColour(juce::TextButton::buttonDown));
    else
      g.setColour(button.findColour(juce::TextButton::buttonNormal));
  }

  g.fillAll();

  if (button.isToggleable() && !button.getToggleState() && !flat) {
    g.setColour(button.findColour(juce::TextButton::buttonColourId)
                    .withMultipliedAlpha(2.0));
    g.drawRect(button.getLocalBounds(), 2);
  }
}

juce::Font CustomLookAndFeel::getLabelFont(juce::Label &) {
  return font.boldened();
}

juce::Font
CustomLookAndFeel::getTextButtonFont(juce::TextButton &button, int height) {
  return font.boldened();
}

void CustomLookAndFeel::drawFileBrowserRow(
    juce::Graphics &g,
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
    juce::DirectoryContentsDisplayComponent &) {
  juce::ignoreUnused(
      file, icon, fileSizeDescription, fileTimeDescription, itemIndex);
  juce::Image image;
  if (isItemSelected) {
    if (isDirectory)
      image = iconFolderSmallOn;
    else
      image = iconFileSmallOn;
    g.fillAll(juce::Colours::white);
    g.setColour(juce::Colours::black);
  } else {
    if (isDirectory)
      image = iconFolderSmall;
    else
      image = iconFileSmall;
    g.setColour(juce::Colours::white);
  }
  image.duplicateIfShared();
  g.drawImageWithin(
      image, 0, 0, height, height, juce::RectanglePlacement::centred);
  g.drawFittedText(
      filename,
      {height, 0, width - height, height},
      juce::Justification::centredLeft,
      1);
}
void CustomLookAndFeel::drawScrollbar(
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
    bool isMouseDown) {
  juce::ignoreUnused(scrollbar, isMouseOver, isMouseDown);
  g.setColour(Constants::WHITE_COLOUR);
  if (isScrollbarVertical)
    g.fillRect(x, thumbStartPosition, width, thumbSize);
  else
    g.fillRect(thumbStartPosition, y, thumbSize, height);
}

void CustomLookAndFeel::drawComboBox(
    juce::Graphics &,
    int width,
    int height,
    bool isButtonDown,
    int buttonX,
    int buttonY,
    int buttonW,
    int buttonH,
    juce::ComboBox &) {}
