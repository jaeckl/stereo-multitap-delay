#include "SingleDelayEditorConfig.h"
#include "Audio/MonoDelayLineConfig.h"
#include "Audio/StereoDelayLineConfig.h"
void SingleDelayEditorConfig::initialize(
    std::shared_ptr<IDelayLineConfig> config,
    PluginProcessor *processor) {
  processorRef = processor;
  if (auto conf = std::dynamic_pointer_cast<MonoDelayLineConfig>(config)) {
    delayLineConfig = conf;
    isStereoDelay = false;
    delayLineConfig->clear();
    delayLineConfig->setPointModel(0, &points);
    return;
  }
  if (auto conf = std::dynamic_pointer_cast<StereoDelayLineConfig>(config)) {
    delayLineConfig = conf;
    isStereoDelay = true;
    delayLineConfig->clear();
    delayLineConfig->setPointModel(0, &points);
    delayLineConfig->setPointModel(1, &points);

    return;
  }
  throw std::exception();
}

XYPointModel &SingleDelayEditorConfig::getPointModel() { return points; }
