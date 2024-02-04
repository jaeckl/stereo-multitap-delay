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
  return nullptr;
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
