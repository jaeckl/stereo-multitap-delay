#pragma once

#include "Components/BypassButton.h"
#include "Components/CustomRotaryKnob.h"
#include "Components/FileSystemButton.h"
#include "Components/IDelayEditorConfig.h"
#include "Components/MultiDelayEditorConfig.h"
#include "Components/SingleDelayEditorConfig.h"
#include "Components/UIElementPlacer.h"

#include "PluginProcessor.h"
#include "Presets/PresetManager.h"
#include "Views/FileBrowserView.h"
#include "Views/XYEditorView.h"
#include <memory>
#include <utility>
#include <vector>
//==============================================================================
class ProcessorEditor final
    : public juce::AudioProcessorEditor
    , private juce::Slider::Listener
    , private juce::Button::Listener
    , private juce::ComboBox::Listener
    , private FileBrowserView::Listener {
public:
  explicit ProcessorEditor(AudioPluginAudioProcessor &);
  ~ProcessorEditor() override;

  //==============================================================================
  void paint(juce::Graphics &) override;
  void resized() override;
  void fileAccepted(const juce::File &file) override;

private:
  juce::FileLogger fileLogger;

  PresetManager presetManager;

  std::shared_ptr<SingleDelayEditorConfig> delayEditorConfigSingle;
  std::shared_ptr<MultiDelayEditorConfig> delayEditorConfigMulti;
  std::shared_ptr<IDelayEditorConfig> activeEditorConfig;

  juce::ImageComponent backgroundImg;

  CustomRotaryKnob wetDrySlider;
  CustomRotaryKnob gainSlider;

  BypassButton bypassButton;

  juce::ComboBox presetComboBox;
  FileSystemButton fileSystemButton;

  XYEditorView xyEditorView;
  FileBrowserView fileBrowserView;

  XMLElementLayouter xmlLayouter;

  void initializeControls();
  void addControlsToView();
  void configureBackgroundImage();
  void configurePresetComboBox();
  void configureRotarySliders();

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

  void switchBetweenModes(bool toggleState);
  void showGroupA();
  void showGroupB();
  void showEditorConfig(std::shared_ptr<IDelayEditorConfig> config);
  std::pair<int, int> computeWetDryRatio(float value);

  void updateAudioBypass();
  void applyPreset(size_t presetId);
  AudioPluginAudioProcessor &processorRef;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProcessorEditor)
};
