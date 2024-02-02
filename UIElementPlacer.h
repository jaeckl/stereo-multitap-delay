#pragma once

#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <unordered_map>
class XMLElementLayouter {
public:
  explicit XMLElementLayouter(juce::Component *component)
      : rootComponent(component) {}
  bool fromDocument(juce::XmlElement *element) {
    if (!element)
      return false;
    for (const auto *child :
         element->getChildWithTagNameIterator("component")) {
      auto id = child->getStringAttribute("id");
      if (id.isEmpty())
        continue;
      auto bounds = getAttributeBounds(*child);
      if (bounds.isEmpty())
        continue;
      elementBounds[id] = bounds;
    }
    return true;
  }
  void updateComponentBounds() {
    for (auto &child : rootComponent->getChildren()) {
      auto it = elementBounds.find(child->getName());
      if (it != elementBounds.end())
        child->setBounds(it->second);
    }
  }

private:
  struct Bounds {
    int x, y, width, height;
  };
  juce::Rectangle<int> getAttributeBounds(const juce::XmlElement &element) {
    const int DEFAULT_VALUE = -1;
    int x = element.getIntAttribute("x", DEFAULT_VALUE);
    int y = element.getIntAttribute("y", DEFAULT_VALUE);
    int width = element.getIntAttribute("width", DEFAULT_VALUE);
    int height = element.getIntAttribute("height", DEFAULT_VALUE);
    if (x < 0 || y < 0 || width < 0 || height < 0)
      return {};
    return {x, y, width, height};
  }
  juce::Component *rootComponent;
  std::unordered_map<juce::String, juce::Rectangle<int>> elementBounds;
};
