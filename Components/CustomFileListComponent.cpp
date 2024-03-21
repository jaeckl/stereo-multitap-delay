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
