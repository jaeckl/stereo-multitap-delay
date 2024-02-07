#include "PluginEditor.h"
#include "BinaryData.h"
#include "Components/FileSystemButton.h"
#include "Components/IDelayEditorConfig.h"
#include "Constants.h"
#include "LookAndFeel/CustomButtonStyle.h"
#include "PluginProcessor.h"
#include "Presets/IPreset.h"
#include "Views/FileBrowserView.h"
#include "Views/XYEditorView.h"
#include <BinaryData.h>
#include <format>
#include <memory>
//==============================================================================
ProcessorEditor::ProcessorEditor(AudioPluginAudioProcessor &p)
    : AudioProcessorEditor(&p)
    , fileLogger(juce::File("LogFile.txt"), "Log File")
    , customStyle()
    , customStyleFlat(true)
    , presetManager()
    , delayEditorConfigSingle(std::make_shared<SingleDelayEditorConfig>())
    , delayEditorConfigMulti(std::make_shared<MultiDelayEditorConfig>())
    , activeEditorConfig(delayEditorConfigSingle)
    , backgroundImg(Constants::COMPONENT_IMAGE_BACKGROUND)
    , wetDrySlider(Constants::COMPONENT_SLIDER_WETDRY, Constants::BLUE_COLOUR)
    , gainSlider(Constants::COMPONENT_SLIDER_GAIN, Constants::BLUE_COLOUR)
    , bypassButton(Constants::COMPONENT_BUTTON_BYPASS)
    , presetComboBox(Constants::COMPONENT_COMBOBOX_PRESETS)
    , fileSystemButton("button-filesystem")
    , xyEditorView("view-xyEditor", this)
    , fileBrowserView("view-fileBrowser")

    , xmlLayouter(this, BinaryData::base_layout_xml)
    , processorRef(p) {

  setResizable(false, false);
  activeEditorConfig->initialize(processorRef.delayLineConfig, &processorRef);
  xyEditorView.setCanvasPointModel(activeEditorConfig.get());
  initializeControls();
  addControlsToView();
}

void ProcessorEditor::initializeControls() {
  configureBackgroundImage();
  configureRotarySliders();
  configurePresetComboBox();
  xyEditorView.setLookAndFeel(&customStyle);
  fileBrowserView.setLookAndFeel(&customStyle);
  fileBrowserView.addListener(this);
  wetDrySlider.addListener(this);
  gainSlider.addListener(this);
  presetComboBox.addListener(this);
  bypassButton.addListener(this);
  fileSystemButton.addListener(this);
}

void ProcessorEditor::addControlsToView() {
  addAndMakeVisible(backgroundImg);

  addAndMakeVisible(wetDrySlider);
  addAndMakeVisible(gainSlider);

  addAndMakeVisible(bypassButton);
  addAndMakeVisible(presetComboBox);
  addAndMakeVisible(fileSystemButton);
  addAndMakeVisible(xyEditorView);
  addAndMakeVisible(fileBrowserView);
  fileBrowserView.setVisible(false);

  xmlLayouter.updateComponentBounds();
}

void ProcessorEditor::configureBackgroundImage() {
  auto imageData = juce::MemoryInputStream(
      BinaryData::plugin_background_png,
      BinaryData::plugin_background_pngSize,
      false);
  auto format = juce::ImageFileFormat::findImageFormatForStream(imageData);
  backgroundImg.setImage(format->decodeImage(imageData));
}
void ProcessorEditor::configurePresetComboBox() {
  presetComboBox.addItemList(presetManager.getPresetNameList(), 1);
}

void ProcessorEditor::configureRotarySliders() {
  configureRotarySlider(
      &wetDrySlider,
      Constants::WETDRY_MIN,
      Constants::WETDRY_MAX,
      Constants::WETDRY_STEP,
      Constants::WETDRY_VALUE,
      Constants::TEXT_UNIT_SLIDER_WETDRY,
      [this](juce::Slider *s) -> juce::String {
        auto ratio = computeWetDryRatio(s->getValue());
        return juce::String(std::format(
            Constants::FORMAT_STRING_WETDRY, ratio.first, ratio.second));
      });
  configureRotarySlider(
      &gainSlider,
      Constants::GAIN_MIN,
      Constants::GAIN_MAX,
      Constants::GAIN_STEP,
      Constants::GAIN_VALUE,
      Constants::TEXT_UNIT_SLIDER_GAIN,
      [](juce::Slider *s) -> juce::String {
        if (s->getValue() >= 0)
          return juce::String(
              std::format(Constants::FORMAT_STRING_GAIN_POS, s->getValue()));
        else
          return juce::String(
              std::format(Constants::FORMAT_STRING_GAIN_NEG, s->getValue()));
      });
}

void ProcessorEditor::configureRotarySlider(
    CustomRotaryKnob *slider,
    float minValue,
    float maxValue,
    float stepSize,
    float value,
    juce::String unitString,
    std::function<juce::String(juce::Slider *)> formater) {
  slider->setRange(minValue, maxValue, stepSize);
  slider->setValue(value);
  slider->setUnitName(unitString);
  slider->setValueFormater(formater);
}

void ProcessorEditor::sliderValueChanged(juce::Slider *slider) {
  if (slider == &wetDrySlider)
    processorRef.delayMix = wetDrySlider.getValue();
  if (slider == &gainSlider)
    processorRef.outputVolume = gainSlider.getValue();
}

void ProcessorEditor::buttonClicked(juce::Button *button) {
  if (button->getName() == Constants::COMPONENT_BUTTON_CHANNEL_SPLIT)
    switchBetweenModes(button->getToggleState());
  if (button->getName() == Constants::COMPONENT_BUTTON_CHANNEL_LEFT &&
      button->getToggleState())
    showGroupA();
  if (button->getName() == Constants::COMPONENT_BUTTON_CHANNEL_RIGHT &&
      button->getToggleState())
    showGroupB();
  if (button == &bypassButton)
    updateAudioBypass();
  if (button == &fileSystemButton) {
    xyEditorView.setVisible(!xyEditorView.isVisible());
    fileBrowserView.setVisible(!fileBrowserView.isVisible());
  }
}

void ProcessorEditor::comboBoxChanged(juce::ComboBox *comboBox) {
  if (comboBox == &presetComboBox) {
    applyPreset(static_cast<size_t>(presetComboBox.getSelectedId() - 1));
  }
}

void ProcessorEditor::applyPreset(size_t presetId) {
  activeEditorConfig->clearPoints();
  auto preset = presetManager.getPreset(presetId);
  activeEditorConfig->clearPoints();
  for (auto &point : preset->getPresetPoints())
    activeEditorConfig->addPoint(point);
  presetComboBox.setSelectedId(0, juce::NotificationType::dontSendNotification);
  repaint();
}

void ProcessorEditor::updateAudioBypass() {
  if (bypassButton.getToggleState())
    processorRef.isBypassing = true;
  else
    processorRef.isBypassing = false;
}

void ProcessorEditor::showEditorConfig(
    std::shared_ptr<IDelayEditorConfig> config) {
  activeEditorConfig = config;

  xyEditorView.setCanvasPointModel(activeEditorConfig.get());
  activeEditorConfig->initialize(processorRef.delayLineConfig, &processorRef);
}

void ProcessorEditor::switchBetweenModes(bool toggleState) {
  if (toggleState) {

    xyEditorView.setChannelButtonsVisible(true);
    showEditorConfig(delayEditorConfigMulti);
    if (xyEditorView.isLeftChannelSelected())
      showGroupA();
    else
      showGroupB();
  } else {
    xyEditorView.setChannelButtonsVisible(false);
    xyEditorView.setCanvasColour(Constants::BLUE_COLOUR);
    showEditorConfig(delayEditorConfigSingle);
  }
  repaint();
}

void ProcessorEditor::showGroupA() {
  xyEditorView.setCanvasColour(Constants::ORGANGE_COLOUR);
  auto *conf = dynamic_cast<MultiDelayEditorConfig *>(activeEditorConfig.get());
  conf->enableGroupA();
  xyEditorView.repaint();
}
void ProcessorEditor::showGroupB() {
  xyEditorView.setCanvasColour(Constants::GREEN_COLOUR);
  auto *conf = dynamic_cast<MultiDelayEditorConfig *>(activeEditorConfig.get());
  conf->enableGroupB();
  xyEditorView.repaint();
}

std::pair<int, int> ProcessorEditor::computeWetDryRatio(float value) {
  return {
      static_cast<int>((1.0f - value) * 100),
      static_cast<int>(std::round(value * 100.0f))};
}

void ProcessorEditor::fileAccepted(const juce::File &file) {
  applyPreset(presetManager.loadPresetFromFile(file));
  // xyEditorView.setVisible(!xyEditorView.isVisible());
  // fileBrowserView.setVisible(!fileBrowserView.isVisible());
  fileSystemButton.setToggleState(
      false, juce::NotificationType::sendNotification);
}

ProcessorEditor::~ProcessorEditor() {}

//==============================================================================
void ProcessorEditor::paint(juce::Graphics &g) { juce::ignoreUnused(g); }
void ProcessorEditor::resized() { xmlLayouter.updateComponentBounds(); }
