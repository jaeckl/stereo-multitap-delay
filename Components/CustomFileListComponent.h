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
