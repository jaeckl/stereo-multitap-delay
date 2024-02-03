#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

class CanvasRuler : public juce::Component {
public:
  explicit CanvasRuler(const juce::String &name);
  enum Alignment { Top, Bottom, Left, Right };
  void setNumTicks(int ticks);
  int getNumTicks();
  void setAlignment(Alignment rulerAlignment);
  void setTickMarkerSize(int length, int width);
  void setTickLabelFunction(std::function<juce::String(int, int)> function);
  void paint(juce::Graphics &g) override;

private:
  Alignment alignment;

  juce::Colour rulerColor = juce::Colours::white;

  int numberOfTicks;
  std::function<juce::String(int, int)> tickLabelFunction;
  int tickMarkerLength = 5;
  int tickMarkerWidth = 2;
};
