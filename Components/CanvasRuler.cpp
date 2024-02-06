#include "CanvasRuler.h"
#include <juce_gui_basics/juce_gui_basics.h>

CanvasRuler::CanvasRuler(const juce::String &name) { setName(name); }

void CanvasRuler::setNumTicks(int ticks) { numberOfTicks = ticks; }
int CanvasRuler::getNumTicks() { return numberOfTicks; }
void CanvasRuler::setAlignment(Alignment rulerAlignment) {
  alignment = rulerAlignment;
}
void CanvasRuler::setTickMarkerSize(int length, int width) {
  tickMarkerLength = length;
  tickMarkerWidth = width;
  axisWidth = width;
}
void CanvasRuler::setTickLabelFunction(
    std::function<juce::String(int, int)> function) {
  tickLabelFunction = function;
}
void CanvasRuler::paint(juce::Graphics &g) {
  // g.setColour(juce::Colours::red);
  // g.fillAll();
  //   render axis
  juce::Font font;
  font.setHeight(12.0f);

  g.setFont(font);
  g.setColour(rulerColor);
  switch (alignment) {
  case Top: {
    drawAxisTop(g);
    drawTicksTop(g);
    drawTickLabelTop(g);
    break;
  }
  case Bottom: {
    drawAxisBottom(g);
    drawTicksBottom(g);
    break;
  }
  case Left: {
    drawAxisLeft(g);
    drawTicksLeft(g);
    break;
  }
  case Right: {
    drawAxisRight(g);
    drawTicksRight(g);
    break;
  }
  }
}
void CanvasRuler::drawAxisTop(juce::Graphics &g) {
  g.fillRect(0, 0, getWidth() - 1, axisWidth);
}
void CanvasRuler::drawAxisBottom(juce::Graphics &g) {
  g.fillRect(0, getHeight() - axisWidth, getWidth(), axisWidth);
}
void CanvasRuler::drawAxisLeft(juce::Graphics &g) {
  g.fillRect(0, 0, axisWidth, getHeight() - 1);
}
void CanvasRuler::drawAxisRight(juce::Graphics &g) {
  g.fillRect(getWidth() - axisWidth, 0, axisWidth, getHeight() - 1);
}

void CanvasRuler::drawTicksTop(juce::Graphics &g) {
  // draw first tick
  g.fillRect(0, 0, tickMarkerWidth, tickMarkerLength);
  int sectionWidth = getWidth() / numberOfTicks;

  for (auto i = 1; i < numberOfTicks; ++i)
    g.fillRect(
        i * sectionWidth - tickMarkerWidth / 2,
        0,
        tickMarkerWidth,
        tickMarkerLength);
  // draw last tick
  g.fillRect(
      getWidth() - tickMarkerWidth, 0, tickMarkerWidth, tickMarkerLength);
}

void CanvasRuler::drawTicksBottom(juce::Graphics &g) {
  g.fillRect(
      0, getHeight() - tickMarkerLength, tickMarkerWidth, tickMarkerLength);
  int sectionWidth = getWidth() / numberOfTicks;

  for (auto i = 1; i < numberOfTicks; ++i)
    g.fillRect(
        i * sectionWidth - tickMarkerWidth / 2,
        getHeight() - tickMarkerLength,
        tickMarkerWidth,
        tickMarkerLength);
  // draw last tick
  g.fillRect(
      getWidth() - tickMarkerWidth,
      getHeight() - tickMarkerLength,
      tickMarkerWidth,
      tickMarkerLength);
}

void CanvasRuler::drawTicksLeft(juce::Graphics &g) {
  g.fillRect(0, 0, tickMarkerLength, tickMarkerWidth);
  int sectionWidth = getHeight() / numberOfTicks;

  for (auto i = 1; i < numberOfTicks; ++i)
    g.fillRect(
        0,
        i * sectionWidth - tickMarkerWidth / 2,
        tickMarkerWidth,
        tickMarkerLength);
  // draw last tick
  g.fillRect(
      0, getHeight() - tickMarkerWidth, tickMarkerLength, tickMarkerWidth);
}

void CanvasRuler::drawTicksRight(juce::Graphics &g) {
  g.fillRect(
      getWidth() - tickMarkerLength, 0, tickMarkerLength, tickMarkerWidth);
  int sectionWidth = getHeight() / numberOfTicks;

  for (auto i = 1; i < numberOfTicks; ++i)
    g.fillRect(
        getWidth() - tickMarkerLength,
        i * sectionWidth - tickMarkerWidth / 2,
        tickMarkerLength,
        tickMarkerWidth);
  // draw last tick
  g.fillRect(
      getWidth() - tickMarkerLength,
      getHeight() - tickMarkerWidth,
      tickMarkerLength,
      tickMarkerWidth);
}

void CanvasRuler::drawTickLabelTop(juce::Graphics &g) {
  // draw first tick
  juce::Font font = g.getCurrentFont();
  auto label = tickLabelFunction(0, numberOfTicks);
  auto textWidth = font.getStringWidth(label);
  g.drawFittedText(
      label.toStdString(),
      0,
      tickMarkerLength,
      textWidth,
      getHeight() - tickMarkerLength,
      juce::Justification::centred,
      1);
  int sectionWidth = getWidth() / numberOfTicks;

  for (auto i = 1; i < numberOfTicks; ++i) {
    label = tickLabelFunction(i, numberOfTicks);
    textWidth = font.getStringWidth(label);
    g.drawFittedText(
        label.toStdString(),
        i * sectionWidth - textWidth / 2,
        tickMarkerLength,
        textWidth,
        getHeight() - tickMarkerLength,
        juce::Justification::centred,
        1);
  }
  label = tickLabelFunction(numberOfTicks, numberOfTicks);
  textWidth = font.getStringWidth(label);
  g.drawFittedText(
      label.toStdString(),
      getWidth() - textWidth,
      tickMarkerLength,
      textWidth,
      getHeight() - tickMarkerLength,
      juce::Justification::centred,
      1);
}
void CanvasRuler::drawTickLabelBottom(juce::Graphics &g) {
  // draw first tick
  juce::Font font = g.getCurrentFont();
  auto label = tickLabelFunction(0, numberOfTicks);
  auto textWidth = font.getStringWidth(label);
  g.drawFittedText(
      label.toStdString(),
      0,
      0,
      textWidth,
      getHeight() - tickMarkerLength,
      juce::Justification::centred,
      1);
  int sectionWidth = getWidth() / numberOfTicks;

  for (auto i = 1; i < numberOfTicks; ++i) {
    label = tickLabelFunction(i, numberOfTicks);
    textWidth = font.getStringWidth(label);
    g.drawFittedText(
        label.toStdString(),
        i * sectionWidth - textWidth / 2,
        0,
        textWidth,
        getHeight() - tickMarkerLength,
        juce::Justification::centred,
        1);
  }
  label = tickLabelFunction(numberOfTicks, numberOfTicks);
  textWidth = font.getStringWidth(label);
  g.drawFittedText(
      label.toStdString(),
      getWidth() - textWidth,
      0,
      textWidth,
      getHeight() - tickMarkerLength,
      juce::Justification::centred,
      1);
}
void CanvasRuler::drawTickLabelLeft(juce::Graphics &g) {
  // draw first tick
  juce::Font font = g.getCurrentFont();
  auto label = tickLabelFunction(0, numberOfTicks);
  auto textWidth = getWidth() - tickMarkerLength;
  auto textHeight = font.getHeight();

  g.drawFittedText(
      label.toStdString(),
      tickMarkerLength,
      0,
      textWidth,
      textHeight,
      juce::Justification::centred,
      1);
  int sectionWidth = getWidth() / numberOfTicks;

  for (auto i = 1; i < numberOfTicks; ++i) {
    label = tickLabelFunction(i, numberOfTicks);
    textWidth = font.getStringWidth(label);
    g.drawFittedText(
        label.toStdString(),
        textWidth,
        i * sectionWidth - textHeight / 2,
        textWidth,
        textHeight,
        juce::Justification::centred,
        1);
  }
  label = tickLabelFunction(numberOfTicks, numberOfTicks);
  textWidth = font.getStringWidth(label);
  g.drawFittedText(
      label.toStdString(),
      textWidth,
      getHeight() - textHeight,
      textWidth,
      textHeight,
      juce::Justification::centred,
      1);
}
void CanvasRuler::drawTickLabelRight(juce::Graphics &g) {
  // draw first tick
  juce::Font font = g.getCurrentFont();
  auto label = tickLabelFunction(0, numberOfTicks);
  auto textWidth = getWidth() - tickMarkerLength;
  auto textHeight = font.getHeight();

  g.drawFittedText(
      label.toStdString(),
      tickMarkerLength,
      0,
      textWidth,
      textHeight,
      juce::Justification::centred,
      1);
  int sectionWidth = getWidth() / numberOfTicks;

  for (auto i = 1; i < numberOfTicks; ++i) {
    label = tickLabelFunction(i, numberOfTicks);
    textWidth = font.getStringWidth(label);
    g.drawFittedText(
        label.toStdString(),
        textWidth,
        i * sectionWidth - textHeight / 2,
        textWidth,
        textHeight,
        juce::Justification::centred,
        1);
  }
  label = tickLabelFunction(numberOfTicks, numberOfTicks);
  textWidth = font.getStringWidth(label);
  g.drawFittedText(
      label.toStdString(),
      textWidth,
      getHeight() - textHeight,
      textWidth,
      textHeight,
      juce::Justification::centred,
      1);
}
