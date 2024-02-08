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
  int axisWidth = 2;

  bool isOverflowRuler = true;

  int canvasOverflowOffset();

  void drawAxisTop(juce::Graphics &g);
  void drawAxisBottom(juce::Graphics &g);
  void drawAxisLeft(juce::Graphics &g);
  void drawAxisRight(juce::Graphics &g);

  void drawTicksTop(juce::Graphics &g);
  void drawTicksBottom(juce::Graphics &g);
  void drawTicksLeft(juce::Graphics &g);
  void drawTicksRight(juce::Graphics &g);

  void drawTickLabelTop(juce::Graphics &g);
  void drawTickLabelBottom(juce::Graphics &g);
  void drawTickLabelLeft(juce::Graphics &g);
  void drawTickLabelRight(juce::Graphics &g);
};
