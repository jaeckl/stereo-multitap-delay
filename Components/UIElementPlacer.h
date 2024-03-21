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

#pragma once

#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <unordered_map>
class XMLElementLayouter {
public:
  explicit XMLElementLayouter(juce::Component *component);
  XMLElementLayouter(juce::Component *component, const char *layoutResource);
  bool fromDocument(juce::XmlElement *element);
  void updateComponentBounds();

private:
  juce::Component *rootComponent;
  int rootWidth, rootHeight;
  std::unordered_map<juce::String, juce::Rectangle<int>> elementBounds;
  juce::Rectangle<int> getAttributeBounds(const juce::XmlElement &element);
};
