#include "CustomFileListComponent.h"
#include "Constants.h"
CustomFileListComponent::CustomFileListComponent(
    const juce::String &name,
    juce::DirectoryContentsList &content)
    : juce::FileListComponent(content) {
  setName(name);
  setColour(
      juce::DirectoryContentsDisplayComponent::highlightColourId,
      Constants::WHITE_COLOUR);
  setColour(
      juce::DirectoryContentsDisplayComponent::highlightedTextColourId,
      juce::Colours::black);
  setColour(juce::ListBox::backgroundColourId, juce::Colours::transparentWhite);
  setRowHeight(24);
}
void CustomFileListComponent::paintListBoxItem(
    int rowNumber,
    juce::Graphics &g,
    int width,
    int height,
    bool rowIsSelected) {
  juce::ignoreUnused(rowNumber, g, width, height, rowIsSelected);
}

void CustomFileListComponent::paintOverChildren(juce::Graphics &g) {
  juce::ignoreUnused(g);
}
