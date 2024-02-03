#pragma once

#include "Components/BypassButton.h"
#include "Components/CanvasRuler.h"
#include "Components/CustomRotaryKnob.h"
#include "Components/IDelayEditorConfig.h"
#include "Components/MultiDelayEditorConfig.h"
#include "Components/SingleDelayEditorConfig.h"
#include "Components/UIElementPlacer.h"
#include "Components/XYPointCanvas.h"
#include "LookAndFeel/CustomButtonStyle.h"
#include "PluginProcessor.h"
#include <memory>
#include <utility>
#include <vector>
//==============================================================================
class ProcessorEditor final
    : public juce::AudioProcessorEditor
    , private juce::Slider::Listener
    , private juce::Button::Listener
    , private juce::ComboBox::Listener {
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

  BypassButton bypassButton;
  juce::TextButton splitChannelsButton;
  juce::TextButton leftChannelButton;
  juce::TextButton rightChannelButton;

  juce::ComboBox gridResolutionComboBox;

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
  void configureGridComboBox();
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
  void comboBoxChanged(juce::ComboBox *comboBox) override;
  void handleDelayComboBox();

  void switchBetweenModes();
  void showGroupA();
  void showGroupB();
  void showEditorConfig(std::shared_ptr<IDelayEditorConfig> config);
  std::pair<int, int> computeWetDryRatio(float value);
  void updateGridResolution(int numTicks);
  void updateAudioBypass();
  AudioPluginAudioProcessor &processorRef;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProcessorEditor)
};
