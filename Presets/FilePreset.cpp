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

#include "FilePreset.h"
FilePreset::FilePreset(
    juce::String name,
    std::vector<juce::Point<float>> points)
    : presetName(name)
    , presetPoints(points) {}

std::unique_ptr<FilePreset>
FilePreset::fromMemory(const char *name, size_t size) {
  return nullptr;
}
std::unique_ptr<FilePreset> FilePreset::fromFile(const juce::File &file) {
  auto stream = file.createInputStream();
  if (stream)
    return fromString(stream->readEntireStreamAsString());
}
std::unique_ptr<FilePreset> FilePreset::fromString(juce::String xmlString) {
  juce::String name;
  std::vector<juce::Point<float>> points;
  auto rootElement = juce::parseXMLIfTagMatches(xmlString, "preset");
  if (!rootElement)
    return nullptr;
  name = rootElement->getStringAttribute("name");

  for (const auto *child : rootElement->getChildWithTagNameIterator("point")) {
    float x = child->getDoubleAttribute("x");
    float y = child->getDoubleAttribute("y");
    points.push_back({x, y});
  }
  return std::unique_ptr<FilePreset>(new FilePreset(name, points));
}
void FilePreset::initialize(double sampleRate, double bpm) {}

const std::vector<juce::Point<float>> &FilePreset::getPresetPoints() const {
  return presetPoints;
}

juce::String FilePreset::getName() { return presetName; }
