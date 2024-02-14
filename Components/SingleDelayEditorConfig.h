#pragma once

#include "Audio/IDelayLineConfig.h"
#include "Components/XYPointModel.h"
#include "IDelayEditorConfig.h"
#include "PluginProcessor.h"
#include <memory>
#include <vector>

class SingleDelayEditorConfig : public IDelayEditorConfig {
private:
  std::shared_ptr<IDelayLineConfig> delayLineConfig;
  AudioPluginAudioProcessor *processorRef;
  XYPointModel points;
  bool isStereoDelay = false;

public:
  void initialize(
      std::shared_ptr<IDelayLineConfig> config,
      AudioPluginAudioProcessor *processor) override;
  XYPointModel &getPointModel() override;
};
