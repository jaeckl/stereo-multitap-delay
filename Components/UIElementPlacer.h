#pragma once

#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <unordered_map>
class XMLElementLayouter {
public:
  explicit XMLElementLayouter(juce::Component *component);
  bool fromDocument(juce::XmlElement *element);
  void updateComponentBounds();

private:
  juce::Component *rootComponent;
  std::unordered_map<juce::String, juce::Rectangle<int>> elementBounds;
  juce::Rectangle<int> getAttributeBounds(const juce::XmlElement &element);
};
