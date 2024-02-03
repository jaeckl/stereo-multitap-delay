#include "PluginEditor.h"
#include "Components/CanvasRuler.h"
#include "Components/CustomRotaryKnob.h"
#include "Components/IDelayEditorConfig.h"
#include "Constants.h"
#include "PluginProcessor.h"
#include <BinaryData.h>
#include <format>
#include <memory>
//==============================================================================
ProcessorEditor::ProcessorEditor(AudioPluginAudioProcessor &p)
    : AudioProcessorEditor(&p)
    , fileLogger(juce::File("LogFile.txt"), "Log File")
    , whiteButtonStyle(Constants::WHITE_COLOUR)
    , blueButtonStyle(Constants::BLUE_COLOUR)
    , orangeButtonStyle(Constants::ORGANGE_COLOUR)
    , greenButtonStyle(Constants::GREEN_COLOUR)
    , backgroundImg(Constants::COMPONENT_IMAGE_BACKGROUND)
    , wetDrySlider(Constants::COMPONENT_SLIDER_WETDRY, Constants::BLUE_COLOUR)
    , gainSlider(Constants::COMPONENT_SLIDER_GAIN, Constants::BLUE_COLOUR)
    , bypassButton("button-bypass")
    , splitChannelsButton(Constants::COMPONENT_BUTTON_CHANNEL_SPLIT)
    , leftChannelButton(Constants::COMPONENT_BUTTON_CHANNEL_LEFT)
    , rightChannelButton(Constants::COMPONENT_BUTTON_CHANNEL_RIGHT)
    , gridSelectLeftButton("button-gridLeft")
    , gridSelectRightButton("button-gridRight")
    , gridLabel("label-gridRes", Constants::GRID_QUATER)
    , gridResolutions(
          {Constants::GRID_HALF,
           Constants::GRID_QUATER,
           Constants::GRID_EIGHTH,
           Constants::GRID_SIXTEENTH})
    , gridResolutionsIndex(1)
    //, gridResolutionComboBox(Constants::COMPONENT_COMBOBOX_GRID_RESOLUTION)
    , pointCanvas(Constants::COMPONENT_CANVAS)
    , horizontalRuler(Constants::COMPONENT_RULER_HORIZONTAL)
    , verticalRuler(Constants::COMPONENT_RULER_VERTICAL)
    , xmlLayouter(this)
    , delayEditorConfigSingle(std::make_shared<SingleDelayEditorConfig>())
    , delayEditorConfigMulti(std::make_shared<MultiDelayEditorConfig>())
    , activeEditorConfig(delayEditorConfigSingle)
    , processorRef(p) {
  // Make sure that before the constructor has finished, you've set the
  // editor's size to whatever you need it to be.
  setSize(Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT);
  setResizable(false, false);
  activeEditorConfig->initialize(processorRef.delayLineConfig, &processorRef);

  initializeControls();
  addControlsToView();
}

void ProcessorEditor::initializeControls() {
  configureBackgroundImage();
  bypassButton.setToggleable(true);
  bypassButton.setClickingTogglesState(true);
  bypassButton.setToggleState(
      false, juce::NotificationType::dontSendNotification);
  configureChannelButtons();
  configureRotarySliders();
  configureRulers();

  wetDrySlider.addListener(this);
  gainSlider.addListener(this);
  bypassButton.addListener(this);
  splitChannelsButton.addListener(this);
  leftChannelButton.addListener(this);
  rightChannelButton.addListener(this);
  leftChannelButton.setToggleState(
      true, juce::NotificationType::dontSendNotification);
  gridSelectLeftButton.addListener(this);
  gridSelectRightButton.addListener(this);
  gridLabel.setJustificationType(juce::Justification::centred);
}

void ProcessorEditor::addControlsToView() {
  addAndMakeVisible(backgroundImg);
  addAndMakeVisible(gridLabel);
  addAndMakeVisible(gridSelectLeftButton);
  addAndMakeVisible(gridSelectRightButton);
  addAndMakeVisible(wetDrySlider);
  addAndMakeVisible(gainSlider);
  addAndMakeVisible(pointCanvas);
  addAndMakeVisible(horizontalRuler);
  addAndMakeVisible(verticalRuler);
  addAndMakeVisible(bypassButton);
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
      BinaryData::plugin_background_png,
      BinaryData::plugin_background_pngSize,
      false);
  auto format = juce::ImageFileFormat::findImageFormatForStream(imageData);
  backgroundImg.setImage(format->decodeImage(imageData));
}

void ProcessorEditor::configureChannelButtons() {
  leftChannelButton.setLookAndFeel(&orangeButtonStyle);
  rightChannelButton.setLookAndFeel(&greenButtonStyle);
  splitChannelsButton.setLookAndFeel(&blueButtonStyle);
  gridSelectLeftButton.setLookAndFeel(&whiteButtonStyle);
  gridSelectRightButton.setLookAndFeel(&whiteButtonStyle);

  leftChannelButton.setClickingTogglesState(true);
  rightChannelButton.setClickingTogglesState(true);
  splitChannelsButton.setClickingTogglesState(true);

  leftChannelButton.setButtonText(Constants::TEXT_BUTTON_CHANNEL_LEFT);
  rightChannelButton.setButtonText(Constants::TEXT_BUTTON_CHANNEL_RIGHT);
  splitChannelsButton.setButtonText(Constants::TEXT_BUTTON_CHANNEL_SPLIT);
  gridSelectLeftButton.setButtonText(Constants::TEXT_BUTTON_GRID_LEFT);
  gridSelectRightButton.setButtonText(Constants::TEXT_BUTTON_GRID_RIGHT);

  leftChannelButton.setRadioGroupId(Constants::RadioGroups::LeftRightChannel);
  rightChannelButton.setRadioGroupId(Constants::RadioGroups::LeftRightChannel);
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
void ProcessorEditor::configureRulers() {
  int ticks = 4;
  pointCanvas.setPointModel(activeEditorConfig.get());
  horizontalRuler.setAlignment(CanvasRuler::Alignment::Top);
  horizontalRuler.setNumTicks(ticks);
  horizontalRuler.setTickMarkerSize(
      Constants::TICK_MARKER_LENGTH, Constants::TICK_MARKER_WIDTH);
  horizontalRuler.setTickLabelFunction([](int idx, int size) -> juce::String {
    if (idx == size)
      return Constants::TEXT_RULER_HORIZONTAL_TICKS_END;
    else
      return std::format(Constants::FORMAT_STRING_BEATS, idx + 1, size);
  });
  verticalRuler.setAlignment(CanvasRuler::Alignment::Right);
  verticalRuler.setNumTicks(Constants::RULER_VERTICAL_NUM_TICKS);
  verticalRuler.setTickMarkerSize(
      Constants::TICK_MARKER_LENGTH, Constants::TICK_MARKER_WIDTH);
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
  if (button == &splitChannelsButton)
    switchBetweenModes();
  if (button == &leftChannelButton && leftChannelButton.getToggleState())
    showGroupA();
  if (button == &rightChannelButton && rightChannelButton.getToggleState())
    showGroupB();
  if (button == &bypassButton)
    updateAudioBypass();
  if (button == &gridSelectLeftButton)
    changeGrid(-1);
  if (button == &gridSelectRightButton)
    changeGrid(+1);
}

void ProcessorEditor::updateAudioBypass() {
  if (bypassButton.getToggleState())
    processorRef.isBypassing = true;
  else
    processorRef.isBypassing = false;
}

void ProcessorEditor::changeGrid(int idx) {
  gridResolutionsIndex = gridResolutionsIndex + idx;
  if (gridResolutionsIndex == 0)
    gridSelectLeftButton.setVisible(false);
  else
    gridSelectLeftButton.setVisible(true);

  if (gridResolutionsIndex == gridResolutions.size() - 1)
    gridSelectRightButton.setVisible(false);
  else
    gridSelectRightButton.setVisible(true);
  gridLabel.setText(
      gridResolutions[gridResolutionsIndex],
      juce::NotificationType::dontSendNotification);
  updateGridResolution(std::pow(2, gridResolutionsIndex + 1));
}

void ProcessorEditor::updateGridResolution(int numTicks) {
  horizontalRuler.setNumTicks(numTicks);
  repaint();
}
void ProcessorEditor::showEditorConfig(
    std::shared_ptr<IDelayEditorConfig> config) {
  activeEditorConfig = config;

  pointCanvas.setPointModel(activeEditorConfig.get());
  activeEditorConfig->initialize(processorRef.delayLineConfig, &processorRef);
}

void ProcessorEditor::switchBetweenModes() {
  if (splitChannelsButton.getToggleState()) {

    leftChannelButton.setVisible(true);
    rightChannelButton.setVisible(true);

    showEditorConfig(delayEditorConfigMulti);
    if (leftChannelButton.getToggleState())
      showGroupA();
    else
      showGroupB();
  } else {
    leftChannelButton.setVisible(false);
    rightChannelButton.setVisible(false);

    pointCanvas.setColour(Constants::BLUE_COLOUR);
    showEditorConfig(delayEditorConfigSingle);
  }
  repaint();
}

void ProcessorEditor::showGroupA() {
  pointCanvas.setColour(Constants::ORGANGE_COLOUR);
  auto *conf = dynamic_cast<MultiDelayEditorConfig *>(activeEditorConfig.get());
  conf->enableGroupA();
  repaint();
}
void ProcessorEditor::showGroupB() {
  pointCanvas.setColour(Constants::GREEN_COLOUR);
  auto *conf = dynamic_cast<MultiDelayEditorConfig *>(activeEditorConfig.get());
  conf->enableGroupB();
  repaint();
}

std::pair<int, int> ProcessorEditor::computeWetDryRatio(float value) {
  return {
      static_cast<int>((1.0f - value) * 100),
      static_cast<int>(std::round(value * 100.0f))};
}

ProcessorEditor::~ProcessorEditor() {}

//==============================================================================
void ProcessorEditor::paint(juce::Graphics &g) { juce::ignoreUnused(g); }
void ProcessorEditor::resized() { xmlLayouter.updateComponentBounds(); }
