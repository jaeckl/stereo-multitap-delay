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

#pragma once
#include "Components/CanvasRuler.h"
#include "Components/UIElementPlacer.h"
#include "Components/XYPointCanvas.h"
#include "Components/XYPointModel.h"
#include "LookAndFeel/OnScreenBorderedLookAndFeel.h"
#include "LookAndFeel/OnScreenFlatLookAndFeel.h"
#include <juce_gui_basics/juce_gui_basics.h>
#include <vector>
class XYEditorView
    : public juce::Component
    , private juce::Button::Listener {

private:
  juce::Button::Listener *buttonListener;

  OnScreenFlatLookAndFeel flatLookAndFeel;
  OnScreenBorderedLookAndFeel borderedLookAndFeel;

  juce::TextButton splitChannelsButton;
  juce::TextButton leftChannelButton;
  juce::TextButton rightChannelButton;

  juce::TextButton gridSelectLeftButton;
  juce::TextButton gridSelectRightButton;
  juce::Label gridLabel;
  std::vector<juce::String> gridResolutions;

  int activeGridIndex;

  XYPointCanvas pointCanvas;
  CanvasRuler horizontalRuler;
  CanvasRuler verticalRuler;

  XMLElementLayouter xmlLayouter;

  void initializeControls();
  void addControlsToView();

  void buttonClicked(juce::Button *) override;

  void configureGridSelectButtons();
  void configureChannelSelectButtons();
  void configureCanvasRulers();

  void changeGrid(int gridIndexDelta);
  void updateGridResolution(int numTicks);

  void setColours(juce::Button &button, juce::Colour colour);

public:
  XYEditorView(
      const juce::String &name,
      juce::Button::Listener *buttonListener);
  void paint(juce::Graphics &g) override;
  void resized() override;
  void setCanvasColour(juce::Colour colour);
  void setCanvasPointModel(XYPointModel *model);
  void setChannelButtonsVisible(bool visible);
  bool isLeftChannelSelected();
};
