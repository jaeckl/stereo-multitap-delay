#pragma once
#include "Components/CanvasRuler.h"
#include "Components/UIElementPlacer.h"
#include "Components/XYPointCanvas.h"
#include "Components/XYPointModel.h"
#include "LookAndFeel/CustomButtonStyle.h"
#include <juce_gui_basics/juce_gui_basics.h>

class XYEditorView
    : public juce::Component
    , private juce::Button::Listener {

private:
  juce::Button::Listener *buttonListener;
  XYPointModel *pointModel;

  CustomButtonStyle whiteButtonStyle;
  CustomButtonStyle blueButtonStyle;
  CustomButtonStyle orangeButtonStyle;
  CustomButtonStyle greenButtonStyle;

  juce::TextButton splitChannelsButton;
  juce::TextButton leftChannelButton;
  juce::TextButton rightChannelButton;

  juce::TextButton gridSelectLeftButton;
  juce::TextButton gridSelectRightButton;
  juce::Label gridLabel;
  std::vector<juce::String> gridResolutions;

  int activeGridIndex;

  std::vector<juce::Point<float>> combinedPoints;
  std::vector<juce::Point<float>> leftChannelPoints;
  std::vector<juce::Point<float>> rightChannelPoints;

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

public:
  XYEditorView(
      const juce::String &name,
      juce::Button::Listener *buttonListener,
      XYPointModel *model);
  void paint(juce::Graphics &g) override;
  void resized() override;
  void setCanvasColour(juce::Colour colour);
  void setCanvasPointModel(XYPointModel *model);
  void setChannelButtonsVisible(bool visible);
  bool isLeftChannelSelected();
};
