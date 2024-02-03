#pragma once

#include "CanvasRuler.h"
#include "CustomRotaryKnob.h"
#include "CustomStyle.h"
#include "DelayEditorConfig.h"
#include "PluginProcessor.h"
#include "UIElementPlacer.h"
#include "XYPointCanvas.h"
#include <memory>
#include <utility>
#include <vector>
//==============================================================================
class ProcessorEditor final
    : public juce::AudioProcessorEditor
    , private juce::Slider::Listener
    , private juce::Button::Listener {
public:
  explicit ProcessorEditor(AudioPluginAudioProcessor &);
  ~ProcessorEditor() override;

  //==============================================================================
  void paint(juce::Graphics &) override;
  void resized() override;

private:
  juce::FileLogger fileLogger;

  CustomButtonStyle blueButtonStyle;
  CustomButtonStyle orangeButtonStyle;
  CustomButtonStyle greenButtonStyle;

  juce::ImageComponent backgroundImg;

  CustomRotaryKnob wetDrySlider;
  CustomRotaryKnob gainSlider;

  juce::TextButton splitChannelsButton;
  juce::TextButton leftChannelButton;
  juce::TextButton rightChannelButton;

  std::vector<juce::Point<float>> combinedPoints;
  std::vector<juce::Point<float>> leftChannelPoints;
  std::vector<juce::Point<float>> rightChannelPoints;

  XYPointCanvas pointCanvas;
  CanvasRuler horizontalRuler;
  CanvasRuler verticalRuler;

  XMLElementLayouter xmlLayouter;
  std::shared_ptr<SingleDelayEditorConfig> delayEditorConfigSingle;
  std::shared_ptr<MultiDelayEditorConfig> delayEditorConfigMulti;
  std::shared_ptr<IDelayEditorConfig> activeEditorConfig;

  void initializeControls();
  void addControlsToView();
  void configureBackgroundImage();
  void configureChannelButtons();
  void configureRotarySliders();
  void configureRulers();

  void configureRotarySlider(
      CustomRotaryKnob *slider,
      float minValue,
      float maxValue,
      float stepSize,
      float value,
      juce::String unitString,
      std::function<juce::String(juce::Slider *)> formater);

  void sliderValueChanged(juce::Slider *slider) override;
  void buttonClicked(juce::Button *button) override;
  void handleDelayComboBox();

  void switchBetweenModes();
  void showGroupA();
  void showGroupB();
  void showEditorConfig(std::shared_ptr<IDelayEditorConfig> config);
  std::pair<int, int> computeWetDryRatio(float value);
  AudioPluginAudioProcessor &processorRef;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProcessorEditor)
};
