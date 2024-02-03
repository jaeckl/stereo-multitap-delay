#include <juce_gui_basics/juce_gui_basics.h>
#include <memory>
class BypassButton : public juce::DrawableButton {
public:
  explicit BypassButton(const juce::String &name);

private:
  std::unique_ptr<juce::Drawable> normalImage;
  std::unique_ptr<juce::Drawable> overImage;
  std::unique_ptr<juce::Drawable> normalImageOn;
  std::unique_ptr<juce::Drawable> overImageOn;

  juce::Image loadImage(const char *name, size_t size);
};
