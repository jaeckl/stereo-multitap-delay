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
  void addDelayLineReader(int channelIndex, int delayInSamples, float value)
      override;
  void setDelayLineValue(
      int channelIndex,
      int delayIndex,
      int delayInSamples,
      float value) override;

  void removeDelayLineReader(int channelIndex, int delayIndex) override;
};
