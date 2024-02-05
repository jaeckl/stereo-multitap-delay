#include "UIElementPlacer.h"
#include <BinaryData.h>
#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>

XMLElementLayouter::XMLElementLayouter(juce::Component *component)
    : rootComponent(component) {}

XMLElementLayouter::XMLElementLayouter(
    juce::Component *component,
    const char *layoutResource)
    : XMLElementLayouter(component) {
  fromDocument(
      juce::parseXMLIfTagMatches(juce::String(layoutResource), "layout").get());
}

bool XMLElementLayouter::fromDocument(juce::XmlElement *element) {
  if (!element)
    return false;
  rootWidth = element->getIntAttribute("width");
  rootHeight = element->getIntAttribute("height");
  for (const auto *child : element->getChildWithTagNameIterator("component")) {
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
void XMLElementLayouter::updateComponentBounds() {
  rootComponent->setSize(rootWidth, rootHeight);
  for (auto &child : rootComponent->getChildren()) {
    auto it = elementBounds.find(child->getName());
    if (it != elementBounds.end())
      child->setBounds(it->second);
  }
}

juce::Rectangle<int>
XMLElementLayouter::getAttributeBounds(const juce::XmlElement &element) {
  const int DEFAULT_VALUE = -1;
  int x = element.getIntAttribute("x", DEFAULT_VALUE);
  int y = element.getIntAttribute("y", DEFAULT_VALUE);
  int width = element.getIntAttribute("width", DEFAULT_VALUE);
  int height = element.getIntAttribute("height", DEFAULT_VALUE);
  if (x < 0 || y < 0 || width < 0 || height < 0)
    return {};
  return {x, y, width, height};
}
