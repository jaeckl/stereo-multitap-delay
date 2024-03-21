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

#include "XYEditorView.h"
#include "Components/XYPointModel.h"
#include "Constants.h"
#include <BinaryData.h>
#include <cmath>
#include <format>
XYEditorView::XYEditorView(
    const juce::String &name,
    juce::Button::Listener *listener)
    : buttonListener(listener)
    , splitChannelsButton(Constants::COMPONENT_BUTTON_CHANNEL_SPLIT)
    , leftChannelButton(Constants::COMPONENT_BUTTON_CHANNEL_LEFT)
    , rightChannelButton(Constants::COMPONENT_BUTTON_CHANNEL_RIGHT)
    , gridSelectLeftButton(Constants::COMPONENT_BUTTON_GRID_LEFT)
    , gridSelectRightButton(Constants::COMPONENT_BUTTON_GRID_RIGHT)
    , gridLabel(
          Constants::COMPONENT_LABEL_GRID_RES,
          Constants::GRID_LABEL + Constants::GRID_QUATER)
    , gridResolutions(
          {Constants::GRID_HALF,
           Constants::GRID_QUATER,
           Constants::GRID_EIGHTH,
           Constants::GRID_SIXTEENTH})
    , activeGridIndex(Constants::GRID_RES_INITIAL_INDEX)
    , pointCanvas(Constants::COMPONENT_CANVAS)
    , horizontalRuler(Constants::COMPONENT_RULER_HORIZONTAL)
    , verticalRuler(Constants::COMPONENT_RULER_VERTICAL)
    , xmlLayouter(this, BinaryData::xyeditor_layout_xml)

{
  setName(name);
  initializeControls();
  addControlsToView();
}

void XYEditorView::initializeControls() {
  configureGridSelectButtons();
  configureChannelSelectButtons();
  configureCanvasRulers();
}

void XYEditorView::addControlsToView() {
  addAndMakeVisible(splitChannelsButton);
  addAndMakeVisible(leftChannelButton);
  leftChannelButton.setVisible(false);
  addAndMakeVisible(rightChannelButton);
  rightChannelButton.setVisible(false);

  addAndMakeVisible(gridLabel);
  addAndMakeVisible(gridSelectLeftButton);
  addAndMakeVisible(gridSelectRightButton);

  addAndMakeVisible(pointCanvas);
  addAndMakeVisible(horizontalRuler);
  addAndMakeVisible(verticalRuler);

  xmlLayouter.updateComponentBounds();
}

void XYEditorView::configureChannelSelectButtons() {
  leftChannelButton.setLookAndFeel(&borderedLookAndFeel);
  rightChannelButton.setLookAndFeel(&borderedLookAndFeel);
  splitChannelsButton.setLookAndFeel(&borderedLookAndFeel);

  setColours(leftChannelButton, Constants::ORGANGE_COLOUR);
  setColours(rightChannelButton, Constants::GREEN_COLOUR);
  setColours(splitChannelsButton, Constants::BLUE_COLOUR);

  leftChannelButton.setClickingTogglesState(true);
  rightChannelButton.setClickingTogglesState(true);
  splitChannelsButton.setClickingTogglesState(true);

  leftChannelButton.setButtonText(Constants::TEXT_BUTTON_CHANNEL_LEFT);
  rightChannelButton.setButtonText(Constants::TEXT_BUTTON_CHANNEL_RIGHT);
  splitChannelsButton.setButtonText(Constants::TEXT_BUTTON_CHANNEL_SPLIT);
  leftChannelButton.setRadioGroupId(Constants::RadioGroups::LeftRightChannel);
  rightChannelButton.setRadioGroupId(Constants::RadioGroups::LeftRightChannel);
  leftChannelButton.setToggleState(
      true, juce::NotificationType::dontSendNotification);

  splitChannelsButton.addListener(buttonListener);
  leftChannelButton.addListener(buttonListener);
  rightChannelButton.addListener(buttonListener);
}

void XYEditorView::configureGridSelectButtons() {
  gridSelectLeftButton.setLookAndFeel(&flatLookAndFeel);
  gridSelectRightButton.setLookAndFeel(&flatLookAndFeel);
  setColours(gridSelectLeftButton, juce::Colours::transparentWhite);
  setColours(gridSelectRightButton, juce::Colours::transparentWhite);

  gridSelectLeftButton.setButtonText(Constants::TEXT_BUTTON_GRID_LEFT);
  gridSelectRightButton.setButtonText(Constants::TEXT_BUTTON_GRID_RIGHT);
  gridLabel.setJustificationType(juce::Justification::centred);

  gridSelectLeftButton.addListener(this);
  gridSelectRightButton.addListener(this);
}

void XYEditorView::configureCanvasRulers() {
  horizontalRuler.setAlignment(CanvasRuler::Alignment::Top);
  updateGridResolution(std::pow(2, activeGridIndex + 1));
  horizontalRuler.setTickMarkerSize(
      Constants::TICK_MARKER_LENGTH, Constants::TICK_MARKER_WIDTH);
  horizontalRuler.setTickLabelFunction([](int idx, int size) -> juce::String {
    if (idx == size)
      return Constants::TEXT_RULER_HORIZONTAL_TICKS_END;
    else
      return std::format(Constants::FORMAT_STRING_BEATS, idx + 1, size);
  });
  verticalRuler.setAlignment(CanvasRuler::Alignment::Right);
  verticalRuler.setNumTicks(2);
  verticalRuler.setTickMarkerSize(
      Constants::TICK_MARKER_LENGTH, Constants::TICK_MARKER_WIDTH);
}

void XYEditorView::buttonClicked(juce::Button *button) {
  if (button == &gridSelectLeftButton)
    changeGrid(-1);
  if (button == &gridSelectRightButton)
    changeGrid(+1);
}

void XYEditorView::changeGrid(int gridIndexDelta) {
  activeGridIndex = activeGridIndex + gridIndexDelta;
  if (activeGridIndex == 0)
    gridSelectLeftButton.setVisible(false);
  else
    gridSelectLeftButton.setVisible(true);

  if (activeGridIndex == gridResolutions.size() - 1)
    gridSelectRightButton.setVisible(false);
  else
    gridSelectRightButton.setVisible(true);
  gridLabel.setText(
      Constants::GRID_LABEL + gridResolutions[activeGridIndex],
      juce::NotificationType::dontSendNotification);
  updateGridResolution(std::pow(2, activeGridIndex + 1));
}

void XYEditorView::updateGridResolution(int numTicks) {
  horizontalRuler.setNumTicks(numTicks);
  repaint();
}

void XYEditorView::setCanvasColour(juce::Colour colour) {
  pointCanvas.setColour(colour);
}

void XYEditorView::setCanvasPointModel(XYPointModel *model) {
  pointCanvas.setPointModel(model);
}

void XYEditorView::setChannelButtonsVisible(bool visible) {
  leftChannelButton.setVisible(visible);
  rightChannelButton.setVisible(visible);
}

bool XYEditorView::isLeftChannelSelected() {
  return leftChannelButton.getToggleState();
}

void XYEditorView::paint(juce::Graphics &g) {
  juce::ignoreUnused(g);
  // grid selecttion group border
  g.setColour(juce::Colours::white);
  g.drawRect(gridLabel.getBounds().expanded(0, 2), 2);
}
void XYEditorView::resized() { xmlLayouter.updateComponentBounds(); }

void XYEditorView::setColours(juce::Button &button, juce::Colour colour) {
  button.setColour(juce::TextButton::buttonColourId, colour.withAlpha(0.1f));
  button.setColour(
      juce::TextButton::buttonOnColourId,
      colour.withAlpha(0.2f).withLightness(0.6));
  button.setColour(
      juce::TextButton::buttonDown, colour.withAlpha(0.2f).withLightness(0.6));

  button.setColour(juce::TextButton::buttonNormal, colour);
  button.setColour(
      juce::TextButton::buttonDown, colour.withLightness(0.4).withAlpha(0.2f));

  button.setColour(juce::TextButton::textColourOnId, juce::Colours::black);
  button.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
}
