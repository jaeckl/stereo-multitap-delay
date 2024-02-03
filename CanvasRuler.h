#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

class CanvasRuler : public juce::Component {
public:
  explicit CanvasRuler(const juce::String &name) { setName(name); }
  enum Alignment { Top, Bottom, Left, Right };
  void setNumTicks(int ticks) { numberOfTicks = ticks; }
  int getNumTicks() { return numberOfTicks; }
  void setAlignment(Alignment rulerAlignment) { alignment = rulerAlignment; }
  void setTickMarkerSize(int length, int width) {
    tickMarkerLength = length;
    tickMarkerWidth = width;
  }
  void setTickLabelFunction(std::function<juce::String(int, int)> function) {
    tickLabelFunction = function;
  }
  void paint(juce::Graphics &g) override {
    // g.setColour(juce::Colours::red);
    // g.fillRect(getLocalBounds());
    //   render axis
    juce::Font font;
    font.setHeight(12.0f);
    g.setFont(font);
    g.setColour(rulerColor);
    switch (alignment) {
    case Top:
      g.drawLine(0, 0, getWidth(), 0, tickMarkerWidth);
      break;
    case Bottom:
      g.drawLine(0, getHeight(), getWidth(), getHeight(), tickMarkerWidth);
      break;
    case Left:
      g.drawLine(0, 0, 0, getHeight(), tickMarkerWidth);
      break;
    case Right:
      g.drawLine(getWidth(), 0, getWidth(), getHeight(), tickMarkerWidth);
      break;
    }
    int tickSpacing = 0;
    int offset2 = 0;
    switch (alignment) {
    case Top:
    case Bottom:
      tickSpacing = getWidth() / (numberOfTicks);
      if (getWidth() % 2 == 0)
        offset2 = 1;
      break;
    case Left:
    case Right:
      tickSpacing = getHeight() / (numberOfTicks);
      if (getHeight() % 2 == 0)
        offset2 = 1;
      break;
    }
    int offset = 0;
    for (auto i = 0; i < numberOfTicks + 1; i++) {
      if (i == 0)
        offset = 1;
      else if (i == numberOfTicks)
        offset = -2 - offset2;
      else
        offset = 0;

      switch (alignment) {
      case Top: {
        g.drawLine(
            i * tickSpacing + offset + i,
            0,
            i * tickSpacing + offset + i,
            tickMarkerLength,
            tickMarkerWidth);
        auto text = tickLabelFunction(i, numberOfTicks);
        int width = g.getCurrentFont().getStringWidth(text);
        int height = getHeight() - tickMarkerLength;
        int x;
        if (i == 0)
          x = 0;
        else if (i == numberOfTicks)
          x = getWidth() - width;
        else
          x = (i * tickSpacing + offset + i) - width / 2;
        int y = tickMarkerLength;
        g.drawFittedText(
            text, x, y, width, height, juce::Justification::centred, 1);
        break;
      }
      case Bottom:
        g.drawLine(
            i * tickSpacing + offset + i,
            getHeight(),
            i * tickSpacing + offset + i,
            getHeight() - tickMarkerLength,
            tickMarkerWidth);
        break;
      case Left:
        g.drawLine(
            0,
            i * tickSpacing + offset + i,
            tickMarkerLength,
            i * tickSpacing + offset + i,
            tickMarkerWidth);
        break;
      case Right:
        g.drawLine(
            getWidth(),
            i * tickSpacing + offset + i,
            getWidth() - tickMarkerLength,
            i * tickSpacing + offset + i,
            tickMarkerWidth);
        break;
      }
    }
  }

private:
  Alignment alignment;

  juce::Colour rulerColor = juce::Colours::white;

  int numberOfTicks;
  std::function<juce::String(int, int)> tickLabelFunction;
  int tickMarkerLength = 5;
  int tickMarkerWidth = 2;
};
