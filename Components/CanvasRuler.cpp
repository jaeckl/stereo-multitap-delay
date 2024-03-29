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

#include "CanvasRuler.h"
#include <cmath>
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
  int offset = canvasOverflowOffset();
  float sectionWidth = static_cast<float>(getWidth() - offset) / numberOfTicks;

  for (auto i = 1; i < numberOfTicks; ++i) {
    float diff = std::round(i * sectionWidth) - (i * sectionWidth);
    int tickCenter = std::round(i * sectionWidth) + offset - diff;
    int tickPosition = tickCenter - tickMarkerWidth / 2;
    g.fillRect(tickPosition, 0, tickMarkerWidth, tickMarkerLength);
  }
  // draw last tick
  g.fillRect(
      getWidth() - tickMarkerWidth, 0, tickMarkerWidth, tickMarkerLength);
}

void CanvasRuler::drawTicksBottom(juce::Graphics &g) {
  g.fillRect(
      0, getHeight() - tickMarkerLength, tickMarkerWidth, tickMarkerLength);
  int offset = canvasOverflowOffset();
  float sectionWidth = static_cast<float>(getWidth() - offset) / numberOfTicks;

  for (auto i = 1; i < numberOfTicks; ++i) {
    float diff = std::round(i * sectionWidth) - (i * sectionWidth);
    int tickCenter = std::round(i * sectionWidth) + offset - diff;
    int tickPosition = tickCenter - tickMarkerWidth / 2;
    g.fillRect(
        tickPosition,
        getHeight() - tickMarkerLength,
        tickMarkerWidth,
        tickMarkerLength);
  }
  // draw last tick
  g.fillRect(
      getWidth() - tickMarkerWidth,
      getHeight() - tickMarkerLength,
      tickMarkerWidth,
      tickMarkerLength);
}

void CanvasRuler::drawTicksLeft(juce::Graphics &g) {
  g.fillRect(0, 0, tickMarkerLength, tickMarkerWidth);

  int offset = canvasOverflowOffset();
  float sectionHeight =
      static_cast<float>(getHeight() - offset) / numberOfTicks;

  for (auto i = 1; i < numberOfTicks; ++i) {
    float diff = std::round(i * sectionHeight) - (i * sectionHeight);
    int tickCenter = std::round(i * sectionHeight) + offset - diff;
    int tickPosition = tickCenter - tickMarkerWidth / 2;
    g.fillRect(0, tickPosition, tickMarkerLength, tickMarkerWidth);
  }
  // draw last tick
  g.fillRect(
      0, getHeight() - tickMarkerWidth, tickMarkerLength, tickMarkerWidth);
}

void CanvasRuler::drawTicksRight(juce::Graphics &g) {
  g.fillRect(
      getWidth() - tickMarkerLength, 0, tickMarkerLength, tickMarkerWidth);
  int offset = canvasOverflowOffset();
  float sectionHeight =
      static_cast<float>(getHeight() - offset) / numberOfTicks;

  for (auto i = 1; i < numberOfTicks; ++i) {
    float diff = std::round(i * sectionHeight) - (i * sectionHeight);
    int tickCenter = std::round(i * sectionHeight) + offset - diff;
    int tickPosition = tickCenter - tickMarkerWidth / 2;
    g.fillRect(
        getWidth() - tickMarkerLength,
        tickPosition,
        tickMarkerLength,
        tickMarkerWidth);
  }
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
  int offset = canvasOverflowOffset();
  int sectionWidth = (getWidth() - offset) / numberOfTicks;

  for (auto i = 1; i < numberOfTicks; ++i) {
    label = tickLabelFunction(i, numberOfTicks);
    textWidth = font.getStringWidth(label);
    float diff = std::round(i * sectionWidth) - (i * sectionWidth);
    int tickCenter = std::round(i * sectionWidth) + offset - diff;
    int textPosition = tickCenter - textWidth / 2;
    g.drawFittedText(
        label.toStdString(),
        textPosition,
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
  int offset = canvasOverflowOffset();
  int sectionWidth = (getWidth() - offset) / numberOfTicks;

  for (auto i = 1; i < numberOfTicks; ++i) {
    label = tickLabelFunction(i, numberOfTicks);
    textWidth = font.getStringWidth(label);
    float diff = std::round(i * sectionWidth) - (i * sectionWidth);
    int tickCenter = std::round(i * sectionWidth) + offset - diff;
    int textPosition = tickCenter - textWidth / 2;
    g.drawFittedText(
        label.toStdString(),
        textPosition,
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
  int offset = canvasOverflowOffset();
  int sectionHeight = (getHeight() - offset) / numberOfTicks;

  for (auto i = 1; i < numberOfTicks; ++i) {
    label = tickLabelFunction(i, numberOfTicks);
    textWidth = font.getStringWidth(label);
    float diff = std::round(i * sectionHeight) - (i * sectionHeight);
    int tickCenter = std::round(i * sectionHeight) + offset - diff;
    int textPosition = tickCenter - textHeight / 2;
    g.drawFittedText(
        label.toStdString(),
        textWidth,
        textPosition,
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
  int offset = canvasOverflowOffset();
  int sectionHeight = (getHeight() - offset) / numberOfTicks;

  for (auto i = 1; i < numberOfTicks; ++i) {
    label = tickLabelFunction(i, numberOfTicks);
    textWidth = font.getStringWidth(label);
    float diff = std::round(i * sectionHeight) - (i * sectionHeight);
    int tickCenter = std::round(i * sectionHeight) + offset - diff;
    int textPosition = tickCenter - textHeight / 2;
    g.drawFittedText(
        label.toStdString(),
        textWidth,
        textPosition,
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

int CanvasRuler::canvasOverflowOffset() {
  if (isOverflowRuler)
    return tickMarkerWidth;
  else
    return 0;
}
