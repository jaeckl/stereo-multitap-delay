#include "MultiDelayEditorConfig.h"
#include "Audio/IDelayLineConfig.h"
#include "Audio/StereoDelayLineConfig.h"
#include "PluginProcessor.h"
#include <exception>
#include <memory>

void MultiDelayEditorConfig::initialize(
    std::shared_ptr<IDelayLineConfig> config,
    PluginProcessor *processor) {
  processorRef = processor;
  if (auto conf = std::dynamic_pointer_cast<StereoDelayLineConfig>(config)) {
    delayLineConfig = conf;
    delayLineConfig->clear();
    delayLineConfig->setPointModel(0, &pointsLeft);
    delayLineConfig->setPointModel(1, &pointsRight);
    return;
  }
  throw std::exception();
}

void MultiDelayEditorConfig::enableGroupA() { isGroupAEnabled = true; }
void MultiDelayEditorConfig::enableGroupB() { isGroupAEnabled = false; }

XYPointModel &MultiDelayEditorConfig::getPointModel() {
  if (isGroupAEnabled)
    return pointsLeft;
  else
    return pointsRight;
}
