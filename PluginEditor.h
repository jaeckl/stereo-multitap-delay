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

#include "Components/BypassButton.h"
#include "Components/CustomRotaryKnob.h"
#include "Components/FileSystemButton.h"
#include "Components/HighpassButton.h"
#include "Components/IDelayEditorConfig.h"
#include "Components/LowpassButton.h"
#include "Components/MultiDelayEditorConfig.h"
#include "Components/SingleDelayEditorConfig.h"
#include "Components/UIElementPlacer.h"
#include "LookAndFeel/OnScreenFlatLookAndFeel.h"
#include "PluginProcessor.h"
#include "Presets/PresetManager.h"
#include "Views/FileBrowserView.h"
#include "Views/XYEditorView.h"
#include <memory>
#include <utility>
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
  OnScreenFlatLookAndFeel onScreenLookAndFeel;
  OnScreenFlatLookAndFeel onScreenFlatLookAndFeel;
  PresetManager presetManager;

  std::shared_ptr<SingleDelayEditorConfig> delayEditorConfigSingle;
  std::shared_ptr<MultiDelayEditorConfig> delayEditorConfigMulti;
  std::shared_ptr<IDelayEditorConfig> activeEditorConfig;

  juce::ImageComponent backgroundImg;
  CustomRotaryKnob wetDrySlider;
  CustomRotaryKnob gainSlider;

  BypassButton bypassButton;

  CustomRotaryKnob filterSlider;

  LowpassButton lowpassButton;
  HighpassButton highpassButton;
  bool lowpassToggled;
  bool highpassToggled;

  juce::ImageComponent stickerComponent;

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
  void configureLowHighPassControls();
  void configureStickerComponent();
  void configureFileBrowserView();
  void configureXYEditorView();

  void configureRotarySlider(
      CustomRotaryKnob *slider,
      float minValue,
      float maxValue,
      float stepSize,
      float value,
      juce::String unitString,
      juce::String titleString,
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
