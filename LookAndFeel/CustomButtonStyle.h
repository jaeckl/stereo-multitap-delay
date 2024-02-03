#include <juce_gui_basics/juce_gui_basics.h>
class CustomButtonStyle : public juce::LookAndFeel_V4 {
public:
  explicit CustomButtonStyle(const juce::Colour &colour);
  void drawButtonBackground(
      juce::Graphics &g,
      juce::Button &button,
      const juce::Colour &backgroundColour,
      bool shoulDrawButtonAsHighlighted,
      bool shouldDrawButtonAsDown) override;
};
