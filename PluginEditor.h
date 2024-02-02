#pragma once

#include "DelayEditorConfig.h"
#include "PluginProcessor.h"
#include "UIElementPlacer.h"
#include "XYPointCanvas.h"
#include <memory>
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

  juce::ImageComponent backgroundImg;

  juce::Slider wetDrySlider;
  juce::Slider gainSlider;

  juce::TextButton splitChannelsButton;
  juce::TextButton leftChannelButton;
  juce::TextButton rightChannelButton;

  std::vector<juce::Point<float>> combinedPoints;
  std::vector<juce::Point<float>> leftChannelPoints;
  std::vector<juce::Point<float>> rightChannelPoints;

  XYPointCanvas pointCanvas;

  XMLElementLayouter xmlLayouter;
  std::shared_ptr<SingleDelayEditorConfig> delayEditorConfigSingle;
  std::shared_ptr<MultiDelayEditorConfig> delayEditorConfigMulti;
  std::shared_ptr<IDelayEditorConfig> activeEditorConfig;

  void initializeControls();
  void addControlsToView();
  void configureBackgroundImage();
  void configureChannelButtons();
  void configureRotarySlider(
      juce::Slider *slider,
      float minValue,
      float maxValue,
      float stepSize,
      float value);

  void sliderValueChanged(juce::Slider *slider) override;
  void buttonClicked(juce::Button *button) override;
  void handleDelayComboBox();

  void showGroupA();
  void showGroupB();
  void showEditorConfig(std::shared_ptr<IDelayEditorConfig> config);

  AudioPluginAudioProcessor &processorRef;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProcessorEditor)
};
