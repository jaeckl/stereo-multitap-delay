#pragma once
#include "Components/XYPointModel.h"
#include <juce_audio_basics/juce_audio_basics.h>
class IDelayLineConfig {
public:
  virtual ~IDelayLineConfig() = default;
  virtual void init(int delayLineSize) = 0;
  virtual void clear() = 0;
  virtual void processBlock(juce::AudioBuffer<float> &buffer) = 0;
  virtual void setPointModel(int channel, XYPointModel *model) = 0;
};
