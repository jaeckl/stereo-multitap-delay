#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include <memory>
class FileSystemButton : public juce::DrawableButton {
public:
  explicit FileSystemButton(const juce::String &name);

private:
  std::unique_ptr<juce::Drawable> normalImage;
  std::unique_ptr<juce::Drawable> overImage;
  std::unique_ptr<juce::Drawable> normalImageOn;
  std::unique_ptr<juce::Drawable> overImageOn;
};
