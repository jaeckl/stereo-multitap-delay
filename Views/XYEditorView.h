#pragma once
#include "Components/CanvasRuler.h"
#include "Components/UIElementPlacer.h"
#include "Components/XYPointCanvas.h"
#include "Components/XYPointModel.h"
#include "LookAndFeel/CustomLookAndFeel.h"
#include <juce_gui_basics/juce_gui_basics.h>

class XYEditorView
    : public juce::Component
    , private juce::Button::Listener {

private:
  juce::Button::Listener *buttonListener;

  CustomLookAndFeel customLookAndFeel;
  CustomLookAndFeel customLookAndFeelFlat;

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
