#pragma once

#include "DelayLine.h"
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_core/juce_core.h>

class IDelayLineConfig {
public:
  virtual ~IDelayLineConfig() = default;
  virtual void init(int delayLineSize) = 0;
  virtual void setDelayLineValue(
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

class MonoDelayLineConfig : public IDelayLineConfig {
private:
  const int CHANNEL_MONO = 0;
  DelayLine<float> delayLine;

public:
  void init(int delayLineSize) override {
    if (delayLineSize != delayLine.getBufferSize())
      delayLine.setBufferSize(delayLineSize);
  }

  void clear() override { delayLine.clear(); }

  void processBlock(juce::AudioBuffer<float> &buffer) override {
    auto *inBuffer = buffer.getReadPointer(CHANNEL_MONO);
    auto *outBuffer = buffer.getWritePointer(CHANNEL_MONO);
    for (auto sampleIndex = 0; sampleIndex < buffer.getNumSamples();
         ++sampleIndex) {
      outBuffer[sampleIndex] = delayLine.processSample(inBuffer[sampleIndex]);
    }
  }
  void addDelayLineReader(int channelIndex, int delayInSamples, float value)
      override {
    juce::ignoreUnused(channelIndex);
    delayLine.addReadingHead(delayInSamples, value);
  }

  void setDelayLineValue(
      int channelIndex,
      int delayIndex,
      int delayInSamples,
      float value) override {
    juce::ignoreUnused(channelIndex);
    delayLine.setReadingHead(delayIndex, delayInSamples, value);
  }

  void removeDelayLineReader(int channelIndex, int delayIndex) override {
    juce::ignoreUnused(channelIndex);
    delayLine.removeReadingHead(delayIndex);
  }
};
class StereoDelayLineConfig : public IDelayLineConfig {
private:
  const int CHANNEL_LEFT = 0;
  const int CHANNEL_RIGHT = 1;
  DelayLine<float> delayLineLeft;
  DelayLine<float> delayLineRight;

public:
  void init(int delayLineSize) override {
    delayLineLeft.setBufferSize(delayLineSize);
    delayLineRight.setBufferSize(delayLineSize);
  }

  void clear() override {
    delayLineLeft.clear();
    delayLineRight.clear();
  }

  void processBlock(juce::AudioBuffer<float> &buffer) override {
    auto *inBufferLeft = buffer.getReadPointer(CHANNEL_LEFT);
    auto *inBufferRight = buffer.getReadPointer(CHANNEL_RIGHT);

    auto *outBufferLeft = buffer.getWritePointer(CHANNEL_LEFT);
    auto *outBufferRight = buffer.getWritePointer(CHANNEL_RIGHT);
    for (auto sampleIndex = 0; sampleIndex < buffer.getNumSamples();
         ++sampleIndex) {
      outBufferLeft[sampleIndex] =
          delayLineLeft.processSample(inBufferLeft[sampleIndex]);
      outBufferRight[sampleIndex] =
          delayLineRight.processSample(inBufferRight[sampleIndex]);
    }
  }
  void addDelayLineReader(int channelIndex, int delayInSamples, float value)
      override {
    if (channelIndex == CHANNEL_LEFT)
      delayLineLeft.addReadingHead(delayInSamples, value);
    if (channelIndex == CHANNEL_RIGHT)
      delayLineRight.addReadingHead(delayInSamples, value);
  }

  void setDelayLineValue(
      int channelIndex,
      int delayIndex,
      int delayInSamples,
      float value) override {
    if (channelIndex == CHANNEL_LEFT)
      delayLineLeft.setReadingHead(delayIndex, delayInSamples, value);
    if (channelIndex == CHANNEL_RIGHT)
      delayLineRight.setReadingHead(delayIndex, delayInSamples, value);
  }

  void removeDelayLineReader(int channelIndex, int delayIndex) override {
    if (channelIndex == CHANNEL_LEFT)
      delayLineLeft.removeReadingHead(delayIndex);
    if (channelIndex == CHANNEL_RIGHT)
      delayLineRight.removeReadingHead(delayIndex);
  }
};
