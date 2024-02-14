#include "PluginEditor.h"
#include "BinaryData.h"
#include "Components/FileSystemButton.h"
#include "Components/HighpassButton.h"
#include "Components/IDelayEditorConfig.h"
#include "Components/LowpassButton.h"
#include "Constants.h"
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
    , onScreenLookAndFeel()
    , onScreenFlatLookAndFeel()
    , presetManager()
    , delayEditorConfigSingle(std::make_shared<SingleDelayEditorConfig>())
    , delayEditorConfigMulti(std::make_shared<MultiDelayEditorConfig>())
    , activeEditorConfig(delayEditorConfigSingle)
    , backgroundImg(Constants::COMPONENT_IMAGE_BACKGROUND)
    , wetDrySlider(Constants::COMPONENT_SLIDER_WETDRY, Constants::BLUE_COLOUR)
    , gainSlider(Constants::COMPONENT_SLIDER_GAIN, Constants::BLUE_COLOUR)
    , bypassButton(Constants::COMPONENT_BUTTON_BYPASS)
    , filterSlider(Constants::COMPONENT_SLIDER_FILTER, Constants::BLUE_COLOUR)
    , lowpassButton(Constants::COMPONENT_BUTTON_LOWPASS)
    , highpassButton(Constants::COMPONENT_BUTTON_HIGHPASS)
    , lowpassToggled(false)
    , highpassToggled(false)
    , stickerComponent("image-sticker")
    , presetComboBox(Constants::COMPONENT_COMBOBOX_PRESETS)
    , fileSystemButton(Constants::COMPONENT_BUTTON_FILESYTEM)
    , xyEditorView(Constants::COMPONENT_VIEW_XYEDITOR, this)
    , fileBrowserView(Constants::COMPONENT_VIEW_FILEBROWSER)

    , xmlLayouter(this, BinaryData::base_layout_xml)
    , processorRef(p) {

  setResizable(false, false);
  activeEditorConfig->initialize(processorRef.delayLineConfig, &processorRef);

  initializeControls();
  addControlsToView();
}

void ProcessorEditor::initializeControls() {
  configureBackgroundImage();
  configureRotarySliders();
  configurePresetComboBox();
  configureLowHighPassControls();
  configureStickerComponent();
  configureFileBrowserView();
  configureXYEditorView();

  bypassButton.addListener(this);
  fileSystemButton.addListener(this);
}

void ProcessorEditor::addControlsToView() {
  addAndMakeVisible(backgroundImg);

  addAndMakeVisible(wetDrySlider);
  addAndMakeVisible(gainSlider);

  addAndMakeVisible(bypassButton);
  addAndMakeVisible(presetComboBox);
  addAndMakeVisible(filterSlider);
  addAndMakeVisible(lowpassButton);
  addAndMakeVisible(highpassButton);
  addAndMakeVisible(stickerComponent);
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
  presetComboBox.setLookAndFeel(&onScreenFlatLookAndFeel);
  presetComboBox.setColour(
      juce::PopupMenu::backgroundColourId, juce::Colours::black);
  presetComboBox.setColour(juce::PopupMenu::textColourId, juce::Colours::white);
  presetComboBox.setColour(
      juce::PopupMenu::highlightedBackgroundColourId, juce::Colours::white);
  presetComboBox.setColour(
      juce::PopupMenu::highlightedTextColourId, juce::Colours::black);
  presetComboBox.getRootMenu();
  presetComboBox.addItemList(presetManager.getPresetNameList(), 1);

  presetComboBox.addListener(this);
}

void ProcessorEditor::configureRotarySliders() {
  configureRotarySlider(
      &wetDrySlider,
      Constants::WETDRY_MIN,
      Constants::WETDRY_MAX,
      Constants::WETDRY_STEP,
      Constants::WETDRY_VALUE,
      Constants::TEXT_UNIT_SLIDER_WETDRY,
      Constants::TEXT_TITLE_WETDRY,
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
      Constants::TEXT_TITLE_GAIN,
      [](juce::Slider *s) -> juce::String {
        if (s->getValue() >= 0)
          return juce::String(std::format(
              Constants::FORMAT_STRING_FLOAT_ZERO_PREC, s->getValue()));
        else
          return juce::String(std::format(
              Constants::FORMAT_STRING_FLOAT_ZERO_PREC, s->getValue()));
      });
  wetDrySlider.addListener(this);
  gainSlider.addListener(this);
}

void ProcessorEditor::configureRotarySlider(
    CustomRotaryKnob *slider,
    float minValue,
    float maxValue,
    float stepSize,
    float value,
    juce::String unitString,
    juce::String titleString,
    std::function<juce::String(juce::Slider *)> formater) {
  slider->setRange(minValue, maxValue, stepSize);
  slider->setValue(value);
  slider->setUnitName(unitString);
  slider->setTitle(titleString);
  slider->setValueFormater(formater);
}
void ProcessorEditor::configureLowHighPassControls() {
  highpassButton.setLookAndFeel(&onScreenFlatLookAndFeel);
  lowpassButton.setLookAndFeel(&onScreenFlatLookAndFeel);
  highpassButton.setRadioGroupId(Constants::HighLowPass);
  lowpassButton.setRadioGroupId(Constants::HighLowPass);
  configureRotarySlider(
      &filterSlider,
      Constants::FILTER_MIN,
      Constants::FILTER_MAX,
      Constants::FILTER_STEP,
      Constants::FILTER_VALUE,
      Constants::TEXT_UNIT_SLIDER_KHZ,
      Constants::TEXT_TITLE_FILTER,
      [&](auto s) -> juce::String {
        float val = std::pow(10, s->getValue());

        if (val >= Constants::FILTER_VALUES_IN_KHZ) {
          filterSlider.setUnitName(Constants::TEXT_UNIT_SLIDER_KHZ);
          return juce::String(std::format(
              Constants::FORMAT_STRING_FLOAT_SINGLE_PREC, val / 1000.0f));
        } else {
          filterSlider.setUnitName(Constants::TEXT_UNIT_SLIDER_HZ);
          if (val > Constants::FILTER_VALUES_WITHOUT_PRECISION)
            return juce::String(
                std::format(Constants::FORMAT_STRING_FLOAT_ZERO_PREC, val));
          else
            return juce::String(
                std::format(Constants::FORMAT_STRING_FLOAT_SINGLE_PREC, val));
        }
      });
  highpassButton.addListener(this);
  lowpassButton.addListener(this);
  filterSlider.addListener(this);
}

void ProcessorEditor::configureStickerComponent() {
  auto inputStream = juce::MemoryInputStream(
      BinaryData::sticker_png, BinaryData::sticker_pngSize, false);
  auto format = juce::ImageFileFormat::findImageFormatForStream(inputStream);
  stickerComponent.setImage(format->decodeImage(inputStream));
}

void ProcessorEditor::configureFileBrowserView() {
  fileBrowserView.setLookAndFeel(&onScreenLookAndFeel);
  fileBrowserView.addListener(this);
}
void ProcessorEditor::configureXYEditorView() {
  xyEditorView.setLookAndFeel(&onScreenFlatLookAndFeel);
  xyEditorView.setCanvasPointModel(&activeEditorConfig->getPointModel());
}

void ProcessorEditor::sliderValueChanged(juce::Slider *slider) {
  if (slider == &wetDrySlider)
    processorRef.delayMix = wetDrySlider.getValue();
  if (slider == &gainSlider)
    processorRef.outputVolume = gainSlider.getValue();
  if (slider == &filterSlider) {
    processorRef.filterValue = slider->getValue();
    processorRef.filterValueChanged = true;
  }
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
  if (button == &lowpassButton) {
    if (lowpassToggled) {
      button->setToggleState(
          false, juce::NotificationType::dontSendNotification);
      processorRef.isLowpassFilter = false;
    } else {
      processorRef.isLowpassFilter = true;
      processorRef.isHighpassFilter = false;
    }
    lowpassToggled = !lowpassToggled;
    processorRef.filterValueChanged = true;
  }
  if (button == &highpassButton) {
    if (highpassToggled) {
      button->setToggleState(
          false, juce::NotificationType::dontSendNotification);
      processorRef.isHighpassFilter = false;
    } else {
      processorRef.isHighpassFilter = true;
      processorRef.isLowpassFilter = false;
    }
    highpassToggled = !highpassToggled;
    processorRef.filterValueChanged = true;
  }
}

void ProcessorEditor::comboBoxChanged(juce::ComboBox *comboBox) {
  if (comboBox == &presetComboBox) {
    applyPreset(static_cast<size_t>(presetComboBox.getSelectedId() - 1));
  }
}

void ProcessorEditor::applyPreset(size_t presetId) {
  activeEditorConfig->getPointModel().clearPoints();
  auto preset = presetManager.getPreset(presetId);
  for (auto &point : preset->getPresetPoints())
    activeEditorConfig->getPointModel().addPoint(point);
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

  xyEditorView.setCanvasPointModel(&activeEditorConfig->getPointModel());
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
  fileSystemButton.setToggleState(
      false, juce::NotificationType::sendNotification);
}

ProcessorEditor::~ProcessorEditor() {}

//==============================================================================
void ProcessorEditor::paint(juce::Graphics &g) { juce::ignoreUnused(g); }
void ProcessorEditor::resized() { xmlLayouter.updateComponentBounds(); }
