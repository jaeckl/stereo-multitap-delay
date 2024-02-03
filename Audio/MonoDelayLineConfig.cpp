#include "MonoDelayLineConfig.h"
void MonoDelayLineConfig::init(int delayLineSize) {
  if (delayLineSize != delayLine.getBufferSize())
    delayLine.setBufferSize(delayLineSize);
}

void MonoDelayLineConfig::clear() { delayLine.clear(); }

void MonoDelayLineConfig::processBlock(juce::AudioBuffer<float> &buffer) {
  auto *inBuffer = buffer.getReadPointer(CHANNEL_MONO);
  auto *outBuffer = buffer.getWritePointer(CHANNEL_MONO);
  for (auto sampleIndex = 0; sampleIndex < buffer.getNumSamples();
       ++sampleIndex) {
    outBuffer[sampleIndex] = delayLine.processSample(inBuffer[sampleIndex]);
  }
}
void MonoDelayLineConfig::addDelayLineReader(
    int channelIndex,
    int delayInSamples,
    float value) {
  juce::ignoreUnused(channelIndex);
  delayLine.addReadingHead(delayInSamples, value);
}

void MonoDelayLineConfig::setDelayLineValue(
    int channelIndex,
    int delayIndex,
    int delayInSamples,
    float value) {
  juce::ignoreUnused(channelIndex);
  delayLine.setReadingHead(delayIndex, delayInSamples, value);
}

void MonoDelayLineConfig::removeDelayLineReader(
    int channelIndex,
    int delayIndex) {
  juce::ignoreUnused(channelIndex);
  delayLine.removeReadingHead(delayIndex);
}
