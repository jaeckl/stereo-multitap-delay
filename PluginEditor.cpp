#include "PluginEditor.h"
#include "DelayEditorConfig.h"
#include "PluginProcessor.h"
#include <BinaryData.h>
#include <memory>

//==============================================================================
ProcessorEditor::ProcessorEditor(AudioPluginAudioProcessor &p)
    : AudioProcessorEditor(&p)
    , fileLogger(juce::File("LogFile.txt"), "Log File")
    , backgroundImg("image-background")
    , wetDrySlider("slider-wetdry")
    , gainSlider("slider-gain")
    , splitChannelsButton("button-splitChannels")
    , leftChannelButton("button-leftChannel")
    , rightChannelButton("button-rightChannel")
    , pointCanvas("component-canvas")
    , xmlLayouter(this)
    , delayEditorConfigSingle(std::make_shared<SingleDelayEditorConfig>())
    , delayEditorConfigMulti(std::make_shared<MultiDelayEditorConfig>())
    , activeEditorConfig(delayEditorConfigSingle)
    , processorRef(p) {
  // Make sure that before the constructor has finished, you've set the
  // editor's size to whatever you need it to be.
  setSize(800, 300);
  setResizable(false, false);
  activeEditorConfig->initialize(processorRef.delayLineConfig, &processorRef);

  initializeControls();
  addControlsToView();
}

void ProcessorEditor::initializeControls() {
  configureBackgroundImage();
  configureChannelButtons();
  configureRotarySlider(&wetDrySlider, 0.0, 1.0, 0.01, 0.5);
  configureRotarySlider(&gainSlider, 0.0, 1.0, 0.01, 1.0);

  pointCanvas.setPointModel(activeEditorConfig.get());

  wetDrySlider.addListener(this);
  gainSlider.addListener(this);

  splitChannelsButton.addListener(this);
  leftChannelButton.addListener(this);
  rightChannelButton.addListener(this);
  leftChannelButton.setToggleState(
      true, juce::NotificationType::dontSendNotification);
}

void ProcessorEditor::addControlsToView() {
  addAndMakeVisible(backgroundImg);
  addAndMakeVisible(wetDrySlider);
  addAndMakeVisible(gainSlider);
  addAndMakeVisible(pointCanvas);
  addAndMakeVisible(splitChannelsButton);
  addAndMakeVisible(leftChannelButton);
  addAndMakeVisible(rightChannelButton);
  leftChannelButton.setVisible(false);
  rightChannelButton.setVisible(false);
  xmlLayouter.fromDocument(juce::parseXML(juce::String(
                                              BinaryData::base_layout_svg,
                                              BinaryData::base_layout_svgSize))
                               .get());
  xmlLayouter.updateComponentBounds();
}

void ProcessorEditor::configureBackgroundImage() {
  auto imageData = juce::MemoryInputStream(
      BinaryData::plugin_background_png, BinaryData::plugin_background_pngSize,
      false);
  auto format = juce::ImageFileFormat::findImageFormatForStream(imageData);
  backgroundImg.setImage(format->decodeImage(imageData));
}

void ProcessorEditor::configureChannelButtons() {
  leftChannelButton.setClickingTogglesState(true);
  leftChannelButton.setButtonText("L");
  rightChannelButton.setClickingTogglesState(true);
  rightChannelButton.setButtonText("R");
  splitChannelsButton.setClickingTogglesState(true);
  splitChannelsButton.setButtonText("X");
  leftChannelButton.setRadioGroupId(1001);
  rightChannelButton.setRadioGroupId(1001);
}

void ProcessorEditor::configureRotarySlider(
    juce::Slider *slider,
    float minValue,
    float maxValue,
    float stepSize,
    float value) {
  slider->setSliderStyle(juce::Slider::RotaryVerticalDrag);
  slider->setRange(minValue, maxValue, stepSize);
  slider->setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
  slider->setValue(value);
}

void ProcessorEditor::sliderValueChanged(juce::Slider *slider) {
  if (slider == &wetDrySlider)
    processorRef.delayMix = wetDrySlider.getValue();
  if (slider == &gainSlider)
    processorRef.outputVolume = gainSlider.getValue();
}

void ProcessorEditor::buttonClicked(juce::Button *button) {
  if (button == &splitChannelsButton) {
    if (splitChannelsButton.getToggleState()) {
      leftChannelButton.setVisible(true);
      rightChannelButton.setVisible(true);

      showEditorConfig(delayEditorConfigMulti);

      if (leftChannelButton.getToggleState()) {
        showGroupA();
      } else {
        showGroupB();
      }
    } else {
      leftChannelButton.setVisible(false);
      rightChannelButton.setVisible(false);

      pointCanvas.setColour(juce::Colour(76, 144, 255));

      showEditorConfig(delayEditorConfigSingle);
    }
    repaint();
  }
  if (button == &leftChannelButton && leftChannelButton.getToggleState()) {
    showGroupA();
  }
  if (button == &rightChannelButton && rightChannelButton.getToggleState()) {
    showGroupB();
  }
}

void ProcessorEditor::showEditorConfig(
    std::shared_ptr<IDelayEditorConfig> config) {
  activeEditorConfig = config;

  pointCanvas.setPointModel(activeEditorConfig.get());
  activeEditorConfig->initialize(processorRef.delayLineConfig, &processorRef);
}

void ProcessorEditor::showGroupA() {
  pointCanvas.setColour(juce::Colour(255, 144, 76));
  auto *conf = dynamic_cast<MultiDelayEditorConfig *>(activeEditorConfig.get());
  conf->enableGroupA();
  repaint();
}
void ProcessorEditor::showGroupB() {
  pointCanvas.setColour(juce::Colour(144, 255, 76));
  auto *conf = dynamic_cast<MultiDelayEditorConfig *>(activeEditorConfig.get());
  conf->enableGroupB();
  repaint();
}

ProcessorEditor::~ProcessorEditor() {}

//==============================================================================
void ProcessorEditor::paint(juce::Graphics &g) { juce::ignoreUnused(g); }
void ProcessorEditor::resized() { xmlLayouter.updateComponentBounds(); }
