#pragma once

#include "Audio/IDelayLineConfig.h"
#include "PluginProcessor.h"
#include "XYPointCanvas.h"
#include <memory>
class IDelayEditorConfig : public XYPointModel {
public:
  virtual void initialize(
      std::shared_ptr<IDelayLineConfig> config,
      AudioPluginAudioProcessor *processor) = 0;
  virtual ~IDelayEditorConfig() = default;
};
