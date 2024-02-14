#pragma once

#include "Audio/IDelayLineConfig.h"
#include "Components/XYPointModel.h"
#include "IDelayEditorConfig.h"
#include "PluginProcessor.h"
#include "XYPointCanvas.h"
#include <juce_gui_basics/juce_gui_basics.h>
#include <memory>
#include <vector>

class MultiDelayEditorConfig : public IDelayEditorConfig {
private:
  std::shared_ptr<IDelayLineConfig> delayLineConfig;
  AudioPluginAudioProcessor *processorRef;
  bool isGroupAEnabled = false;
  XYPointModel pointsLeft;
  XYPointModel pointsRight;

public:
  void initialize(
      std::shared_ptr<IDelayLineConfig> config,
      AudioPluginAudioProcessor *processor) override;
  XYPointModel &getPointModel() override;
  void enableGroupA();
  void enableGroupB();
};
