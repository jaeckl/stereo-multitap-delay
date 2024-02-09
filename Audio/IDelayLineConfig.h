#pragma once
#include <juce_audio_basics/juce_audio_basics.h>
class IDelayLineConfig {
public:
  virtual ~IDelayLineConfig() = default;
  virtual void init(int delayLineSize) = 0;
  virtual void setDelayLineValue(
      int channelIndex,
      int delayIndex,
      int delayInSamples,
      float value) = 0;
  virtual void insertDelayLineValue(
      int channelIndex,
      int delayIndex,
      int delayInSamples,
      float value) = 0;
  virtual void
  addDelayLineReader(int channelIndex, int delayInSamples, float value) = 0;
  virtual void removeDelayLineReader(int channelIndex, int delayIndex) = 0;
  virtual void clear() = 0;
  virtual void processBlock(juce::AudioBuffer<float> &buffer) = 0;
};
