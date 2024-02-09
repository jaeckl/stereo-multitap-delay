#include "OnScreenFlatLookAndFeel.h"
#include <BinaryData.h>
OnScreenFlatLookAndFeel::OnScreenFlatLookAndFeel() {
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
  iconDropDown = loadImage(
      BinaryData::icon_dropdown_png, BinaryData::icon_dropdown_pngSize);
}
juce::Image OnScreenFlatLookAndFeel::loadImage(
    const char *resourceName,
    size_t resourceSize) {
  auto instream = juce::MemoryInputStream(resourceName, resourceSize, false);
  auto format = juce::ImageFileFormat::findImageFormatForStream(instream);
  return format->decodeImage(instream);
}
void OnScreenFlatLookAndFeel::drawButtonBackground(
    juce::Graphics &g,
    juce::Button &button,
    const juce::Colour &backgroundColour,
    bool shoulDrawButtonAsHighlighted,
    bool shouldDrawButtonAsDown) {
  juce::ignoreUnused(backgroundColour, shoulDrawButtonAsHighlighted);
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
}

void OnScreenFlatLookAndFeel::drawFileBrowserRow(
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
    juce::DirectoryContentsDisplayComponent &dcc) {
  juce::ignoreUnused(
      file, icon, fileSizeDescription, fileTimeDescription, itemIndex);
  auto fileListComp = dynamic_cast<juce::Component *>(&dcc);
  juce::Image image;
  if (isItemSelected) {
    if (isDirectory)
      image = iconFolderSmallOn;
    else
      image = iconFileSmallOn;
    g.fillAll(fileListComp->findColour(
        juce::DirectoryContentsDisplayComponent::highlightColourId));
    g.setColour(fileListComp->findColour(
        juce::DirectoryContentsDisplayComponent::highlightedTextColourId));
  } else {
    if (isDirectory)
      image = iconFolderSmall;
    else
      image = iconFileSmall;
    g.setColour(fileListComp->findColour(
        juce::DirectoryContentsDisplayComponent::textColourId));
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
void OnScreenFlatLookAndFeel::drawScrollbar(
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
  g.setColour(scrollbar.findColour(juce::ScrollBar::thumbColourId));
  if (isScrollbarVertical)
    g.fillRect(x, thumbStartPosition, width, thumbSize);
  else
    g.fillRect(thumbStartPosition, y, thumbSize, height);
}
void OnScreenFlatLookAndFeel::drawComboBox(
    juce::Graphics &g,
    int width,
    int height,
    bool isButtonDown,
    int buttonX,
    int buttonY,
    int buttonW,
    int buttonH,
    juce::ComboBox &comboBox) {
  juce::ignoreUnused(width, height, isButtonDown, comboBox);
  g.drawImageWithin(
      iconDropDown,
      buttonX,
      buttonY,
      buttonW,
      buttonH,
      juce::RectanglePlacement::centred);
}
void OnScreenFlatLookAndFeel::drawPopupMenuBackground(
    juce::Graphics &g,
    int width,
    int height) {
  juce::ignoreUnused(width, height);
  g.fillAll(juce::Colours::black);
}
const juce::Image *OnScreenFlatLookAndFeel::getDefaultDirectoryImage() {
  return &iconFolderSmall;
}
const juce::Image *OnScreenFlatLookAndFeel::getDefaultDirectoryImageOn() {
  return &iconFolderSmallOn;
}
const juce::Image *OnScreenFlatLookAndFeel::getDefaultFileImage() {
  return &iconFileSmall;
}
const juce::Image *OnScreenFlatLookAndFeel::getDefaultFileImageOn() {
  return &iconFileSmallOn;
}
