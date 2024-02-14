#pragma once
#include "DelayLine.h"
#include "IDelayLineConfig.h"
class MonoDelayLineConfig : public IDelayLineConfig {
private:
  const int CHANNEL_MONO = 0;
  DelayLine<float> delayLine;

public:
  void init(int delayLineSize) override;
  void clear() override;
  void processBlock(juce::AudioBuffer<float> &buffer) override;
  void setPointModel(int channel, XYPointModel *model) override;
};
