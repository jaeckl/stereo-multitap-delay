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
#include <juce_gui_basics/juce_gui_basics.h>
class CustomFileListComponent : public juce::FileListComponent {
public:
  CustomFileListComponent(
      const juce::String &name,
      juce::DirectoryContentsList &content);
  void paintListBoxItem(
      int rowNumber,
      juce::Graphics &g,
      int width,
      int height,
      bool rowIsSelected) override;
  void paintOverChildren(juce::Graphics &) override;
};
