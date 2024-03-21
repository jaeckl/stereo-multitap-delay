#pragma once

#include "Audio/IDelayLineConfig.h"
#include "Components/XYPointModel.h"
#include "PluginProcessor.h"
#include <memory>
class IDelayEditorConfig {
public:
  virtual void initialize(
      std::shared_ptr<IDelayLineConfig> config,
      PluginProcessor *processor) = 0;
  virtual ~IDelayEditorConfig() = default;
  virtual XYPointModel &getPointModel() = 0;
};
